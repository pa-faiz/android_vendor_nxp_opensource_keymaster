/*
**
** Copyright 2018, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/
/******************************************************************************
 **
 ** The original Work has been changed by NXP.
 **
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 **
 ** http://www.apache.org/licenses/LICENSE-2.0
 **
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 **
 ** Copyright 2020 NXP
 **
 *********************************************************************************/
#define LOG_TAG "OmapiTransport"

#include <log/log.h>
#include <iomanip>
#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <vector>
#include "AppletConnection.h"
#include "EseTransportUtils.h"

using ::android::hardware::secure_element::V1_0::SecureElementStatus;
using ::android::hardware::secure_element::V1_0::LogicalChannelResponse;
using android::base::StringPrintf;

namespace se_transport {
class SecureElementCallback : public ISecureElementHalCallback {
 public:
    Return<void> onStateChange(bool state) override {
        mSEClientState = state;
        return Void();
    };
    Return<void> onStateChange_1_1(bool state, const hidl_string& reason) override {
        LOG(INFO) << __FUNCTION__ << " connected =" << (state?"true " : "false " ) << "reason: " << reason;
        mSEClientState = state;
        return Void();
    };
    bool isClientConnected() {
        return mSEClientState;
    }
 private:
    bool mSEClientState = false;
};

sp<SecureElementCallback> mCallback = nullptr;

//TODO: Need to remove once keymaster supports logical channel
std::vector<uint8_t> weaverAID = {0xA0, 0x00, 0x00, 0x03,
                                         0x96, 0x10, 0x10};

bool AppletConnection::connectToSEService() {
    if (mSEClient != nullptr && mCallback->isClientConnected()) {
        LOG(INFO) <<"Already connected";
        return true;
    }

    uint8_t retry = 0;
    while (( mSEClient == nullptr ) && retry++ < MAX_GET_SERVICE_RETRY ){ // How long should we try before giving up !
      mSEClient = ISecureElement::tryGetService("eSE1");

      if(mSEClient == nullptr){
        LOG(ERROR) << "failed to get eSE HAL service : retry after 1 sec , retry cnt = " << android::hardware::toString(retry) ;
      }else {
        LOG(INFO) << " !!! SuccessFully got Handle to eSE HAL service" ;
        if (mCallback == nullptr) {
          mCallback = new SecureElementCallback();
        }
        mSEClient->init_1_1(mCallback);
        break;
      }
      usleep(ONE_SEC);
    }
    return true;
}

bool AppletConnection::openChannelToApplet(std::vector<uint8_t>& resp) {
    bool ret = true;
    if (isChannelOpen()) {
        //close();
        LOG(INFO) << "channel Already opened";
        return true;
    }
    if (mSEClient == nullptr || !mCallback->isClientConnected()) {  // Not connected to SE service
        LOG(ERROR) << "Not connected to eSE Service";
        return false;
    }
    //TODO: Need to remove if check and else code once keymaster supports logical channel
    if(kAppletAID == weaverAID) {
      mSEClient->openLogicalChannel(kAppletAID, 00,
          [&](LogicalChannelResponse selectResponse, SecureElementStatus status) {
          if (status == SecureElementStatus::SUCCESS) {
          resp = selectResponse.selectResponse;
          mOpenChannel = selectResponse.channelNumber;
          }else {
          ret = false;
          }
          LOG(INFO) << "openLogicalChannel returned: " << toString(status) << " channelNumber =" <<                                                                            ::android::hardware::toString(selectResponse.channelNumber) << " " << selectResponse.selectResponse;
          });
    } else {
      SecureElementStatus statusReturned;
      std::vector<uint8_t> response;
      mSEClient->openBasicChannel(kAppletAID, 0x00,
          [&](std::vector<uint8_t> selectResponse,
            SecureElementStatus status) {
          statusReturned = status;
          if (status == SecureElementStatus::SUCCESS) {
          response.resize(selectResponse.size());
          for (size_t i = 0; i < selectResponse.size(); i++) {
          response[i] = selectResponse[i];
          }
          } else {
          ret = false;
          //resp = {};
          }
          LOG(INFO) << "openBasicChannel returned: " << toString(status);
          });
      mOpenChannel = 0;
      resp = response;
    }
    return ret;
}

bool AppletConnection::transmit(std::vector<uint8_t>& CommandApdu , std::vector<uint8_t>& output){
  hidl_vec<uint8_t> cmd = CommandApdu;
  //TODO: Need to remove if check once keymaster supports logical channel
  if(kAppletAID == weaverAID) {
    cmd[0] |= mOpenChannel ;
    LOG(INFO) << __FUNCTION__ << " on channel number " << ::android::hardware::toString(mOpenChannel);
  }
  mSEClient->transmit(cmd, [&](hidl_vec<uint8_t> result) {
      output = result;
      LOG(INFO) << "recieved response size = " << ::android::hardware::toString(result.size()) << " data = " << result;
      });
  return true;
}

bool AppletConnection::close() {
    if (!isChannelOpen() || mSEClient == nullptr) {
         LOG(ERROR) << "Channel couldn't be closed mSEClient handle is null";
         return false;
    }

    SecureElementStatus status = mSEClient->closeChannel(mOpenChannel);
    if (status != SecureElementStatus::SUCCESS) {
         LOG(ERROR) << "Channel couldn't be closed: closeChannel call failed";
         return false;
    }
    LOG(INFO) << "Channel closed";
    mOpenChannel = -1;
    return true;
}

bool AppletConnection::isChannelOpen() {
    return mOpenChannel >= 0;
}

}  // namespace se_transport
