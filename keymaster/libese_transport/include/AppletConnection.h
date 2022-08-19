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
 ** Copyright 2020-2022 NXP
 **
 *********************************************************************************/
#ifndef __APPLETCONNECTION_H__
#define __APPLETCONNECTION_H__

#include <android/hardware/secure_element/1.2/ISecureElement.h>
#include <android/hardware/secure_element/1.1/ISecureElementHalCallback.h>
#include <android/hardware/secure_element/1.0/types.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <vector>

#include <IntervalTimer.h>
#define BEGIN_OPERATION_CMD (0x30)   // begin()
#define UPDATE_OPERATION_CMD (0x30)  // update()
#define FINISH_OPERATION_CMD (0x32)  // finish()
#define ABORT_OPERATION_CMD (0x33)   // abort()

#define REGULAR_SESSION_TIMEOUT (3 * 1000)     // 3 secs,default value
#define CRYPTO_OP_SESSION_TIMEOUT (20 * 1000)  // 20 secs,for crypto operation

namespace se_transport {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;
using ::android::hardware::secure_element::V1_2::ISecureElement;
using ::android::hardware::secure_element::V1_1::ISecureElementHalCallback;

struct AppletConnection {
public:
  AppletConnection(const std::vector<uint8_t>& aid);

  /**
   * Connects to the secure element HAL service. Returns true if successful, false otherwise.
   */
  bool connectToSEService();

  /**
   * Select the applet on the secure element. SELECT command response is returned in resp vector
   */
  bool openChannelToApplet(std::vector<uint8_t>& resp);

  /**
   * If open, closes the open channel to the applet. Returns an error if channel was not
   * open or the SE HAL service returned an error.
   */
  bool close();

  /**
   * Sends the data to the secure element and also receives back the data.
   * This is a blocking call.
   */
  bool transmit(std::vector<uint8_t>& CommandApdu, std::vector<uint8_t>& output);

  /**
   * Checks if a chennel to the applet is open.
   */
  bool isChannelOpen();

  /**
     * Provides session timeout value for Logical channel mgmt
     * 1) CRYPTO_OP_SESSION_TIMEOUT for crypto begin()
     * 2) REGULAR_SESSION_TIMEOUT for all other operations
     * Params : void
     * Returns : Session timeout value in ms
     */
    int getSessionTimeout();


private:
    std::mutex channel_mutex_; // exclusive access to isChannelopen()/close()
    sp<ISecureElement> mSEClient;
    std::vector<uint8_t> kAppletAID;
    IntervalTimer mTimerCrypto;  // track crypto operations
    int8_t mOpenChannel = -1;

    void startTimer(bool isStart, IntervalTimer& t, int timeout,
                    void (*timerFunc)(union sigval arg));
};

}  // namespace se_transport
#endif  // __APPLETCONNECTION_H__
