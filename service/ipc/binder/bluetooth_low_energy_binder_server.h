//
//  Copyright (C) 2015 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#pragma once

#include <memory>

#include <base/macros.h>

#include "service/common/bluetooth/binder/IBluetoothLowEnergy.h"
#include "service/common/bluetooth/binder/IBluetoothLowEnergyCallback.h"
#include "service/common/bluetooth/low_energy_constants.h"
#include "service/ipc/binder/interface_with_instances_base.h"
#include "service/low_energy_client.h"

namespace bluetooth {
class Adapter;
}  // namespace bluetooth

namespace ipc {
namespace binder {

// Implements the server side of the IBluetoothLowEnergy interface.
class BluetoothLowEnergyBinderServer
    : public BnBluetoothLowEnergy,
      public InterfaceWithInstancesBase,
      public bluetooth::LowEnergyClient::Delegate {
 public:
  explicit BluetoothLowEnergyBinderServer(bluetooth::Adapter* adapter);
  ~BluetoothLowEnergyBinderServer() override;

  // IBluetoothLowEnergy overrides:
  bool RegisterClient(
      const android::sp<IBluetoothLowEnergyCallback>& callback) override;
  void UnregisterClient(int client_id) override;
  void UnregisterAll() override;
  bool StartScan(
      int client_id,
      const bluetooth::ScanSettings& settings,
      const std::vector<bluetooth::ScanFilter>& filters) override;
  bool StopScan(int client_id) override;
  bool StartMultiAdvertising(
      int client_id,
      const bluetooth::AdvertiseData& advertise_data,
      const bluetooth::AdvertiseData& scan_response,
      const bluetooth::AdvertiseSettings& settings) override;
  bool StopMultiAdvertising(int client_id) override;

  // bluetooth::LowEnergyClient::Delegate overrides:
  void OnScanResult(bluetooth::LowEnergyClient* client,
                    const bluetooth::ScanResult& result) override;

 private:
  // Returns a pointer to the IBluetoothLowEnergyCallback instance associated
  // with |client_id|. Returns NULL if such a callback cannot be found.
  android::sp<IBluetoothLowEnergyCallback> GetLECallback(int client_id);

  // Returns a pointer to the LowEnergyClient instance associated with
  // |client_id|. Returns NULL if such a client cannot be found.
  std::shared_ptr<bluetooth::LowEnergyClient> GetLEClient(int client_id);

  // InterfaceWithInstancesBase override:
  void OnRegisterInstanceImpl(
      bluetooth::BLEStatus status,
      android::sp<IInterface> callback,
      bluetooth::BluetoothInstance* instance) override;

  bluetooth::Adapter* adapter_;  // weak

  DISALLOW_COPY_AND_ASSIGN(BluetoothLowEnergyBinderServer);
};

}  // namespace binder
}  // namespace ipc