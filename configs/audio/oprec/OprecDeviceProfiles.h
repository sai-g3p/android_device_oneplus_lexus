/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <OprecProfileConfig.h>
#include "OprecProfiles.h"

namespace oprec {

inline Selection selectProfile(const Capture& capture) {
    uint32_t record_type = capture.low_latency ? 1 : 0;
    if (capture.source == AUDIO_SOURCE_CAMCORDER && capture.channels > 2) {
        record_type = 3;
    }

    const bool is_builtin = capture.device == AUDIO_DEVICE_IN_BUILTIN_MIC ||
                            capture.device == AUDIO_DEVICE_IN_BACK_MIC;
    const bool is_wired = capture.device == AUDIO_DEVICE_IN_WIRED_HEADSET;
    const bool is_usb = capture.device == AUDIO_DEVICE_IN_USB_DEVICE ||
                        capture.device == AUDIO_DEVICE_IN_USB_HEADSET;
    const bool is_bt = capture.device == AUDIO_DEVICE_IN_BLUETOOTH_SCO_HEADSET ||
                       capture.device == AUDIO_DEVICE_IN_BLUETOOTH_BLE;

    int32_t profile;
    if (is_builtin) {
        if (capture.scene == Scene::INTERVIEW) return {profiles::MIC_INTERVIEW, record_type};
        if (capture.scene == Scene::CONFERENCE) return {profiles::MIC_CONFERENCE, record_type};
        if (capture.source == AUDIO_SOURCE_UNPROCESSED) return {profiles::MIC_UNPROCESS, record_type};
        if (capture.low_latency) return {profiles::MIC_FAST, record_type};

        switch (capture.source) {
            case AUDIO_SOURCE_CAMCORDER:
                profile = profiles::MIC_CAM;
                break;
            case AUDIO_SOURCE_VOICE_RECOGNITION:
                profile = profiles::MIC_VR;
                break;
            case AUDIO_SOURCE_VOICE_COMMUNICATION:
                profile = profiles::MIC_VOICE;
                break;
            default:
                profile = profiles::MIC_NORMAL;
                break;
        }

        if (capture.scene == Scene::APP1 && capture.sample_rate == 16000) {
            profile = profiles::MIC_APP1;
        }
    } else if (is_wired) {
        if (capture.scene == Scene::INTERVIEW) return {profiles::HE_INTERVIEW, record_type};
        if (capture.scene == Scene::CONFERENCE) return {profiles::HE_CONFERENCE, record_type};
        if (capture.source == AUDIO_SOURCE_UNPROCESSED) return {profiles::HE_UNPROCESS, record_type};
        if (capture.low_latency) return {profiles::HE_FAST, record_type};

        switch (capture.source) {
            case AUDIO_SOURCE_CAMCORDER:
                profile = profiles::HE_CAM;
                break;
            case AUDIO_SOURCE_VOICE_RECOGNITION:
                profile = profiles::HE_VR;
                break;
            case AUDIO_SOURCE_VOICE_COMMUNICATION:
                profile = profiles::HE_VOICE;
                break;
            default:
                profile = profiles::HE;
                break;
        }

        if (capture.scene == Scene::APP1 && capture.sample_rate == 16000) {
            profile = profiles::HE_APP1;
        }
    } else if (is_usb) {
        if (capture.scene == Scene::INTERVIEW) return {profiles::HE_INTERVIEW, record_type};
        if (capture.scene == Scene::CONFERENCE) return {profiles::HE_CONFERENCE, record_type};

        switch (capture.source) {
            case AUDIO_SOURCE_CAMCORDER:
                profile = profiles::HE_CAM;
                break;
            case AUDIO_SOURCE_VOICE_RECOGNITION:
                profile = profiles::HE_VR;
                break;
            default:
                profile = profiles::HE_USB;
                break;
        }

        if (capture.scene == Scene::APP1 && capture.sample_rate == 16000) {
            profile = profiles::HE_APP1_USB;
        }
    } else if (is_bt) {
        profile = profiles::BT_NORMAL;
        if (capture.scene == Scene::APP1 && capture.sample_rate == 16000) {
            profile = profiles::BT_APP1;
        }
    } else {
        return {-1, record_type};
    }

    return {profile, record_type};
}

}  // namespace oprec
