/*
 * Copyright 2018 Metrological
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../Report.h"
#include <interfaces/IDRM.h> 
#include "MediaSessionSystem.h"

namespace CDMi {

namespace {

    class CCLInitialize {
        CCLInitialize(const CCLInitialize&) = delete;
        CCLInitialize& operator= (const CCLInitialize&) = delete;

    public:
        CCLInitialize() {
            bool result = nvInitialize();
            if ( result == false ) {
                REPORT("Call to nvInitialize failed");
            }
        }

        ~CCLInitialize() {
            nvTerminate();
        }

    };

    static CCLInitialize g_CCLInit;

}


class NagraSystem : public IMediaKeys {
private:
    NagraSystem(const NagraSystem&) = delete;
    NagraSystem& operator= (const NagraSystem&) = delete;

public:
    NagraSystem() {
    }
    ~NagraSystem(void) {
    }

    CDMi_RESULT CreateMediaKeySession(
        int32_t licenseType,
        const char *f_pwszInitDataType,
        const uint8_t *f_pbInitData,
        uint32_t f_cbInitData, 
        const uint8_t *f_pbCDMData,
        uint32_t f_cbCDMData, 
        IMediaKeySession **f_ppiMediaKeySession) {

        *f_ppiMediaKeySession = CDMi::MediaSessionSystem::CreateMediaSessionSystem(f_pbInitData, f_cbInitData);
 
        return CDMi_SUCCESS; 
    }

    CDMi_RESULT SetServerCertificate(
        const uint8_t *f_pbServerCertificate,
        uint32_t f_cbServerCertificate) {

        return CDMi_S_FALSE;
    }

    CDMi_RESULT DestroyMediaKeySession(IMediaKeySession* f_piMediaKeySession) {

        CDMi::MediaSessionSystem::DestroyMediaSessionSystem(f_piMediaKeySession);

        return CDMi_SUCCESS; 
    }
};

static SystemFactoryType<NagraSystem> g_instanceSystem({"video/x-h264", "audio/mpeg"});

}  // namespace CDMi

CDMi::ISystemFactory* GetSystemFactory() {

    return (&CDMi::g_instanceSystem); 
}