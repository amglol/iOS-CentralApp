//
//  PeripheralManager.swift
//  iOS-Central-App
//
//  Created by Adrian Galecki on 7/16/20.
//  Copyright © 2020 Adrian Galecki. All rights reserved.
//

import UIKit
import CoreBluetooth

class PeripheralManager {
    static let peripheralManagerInstance = PeripheralManager()
    
    var storedPeripherals = [CBPeripheral]()
    
    func GetStoredPeripherals() -> [CBPeripheral] {
        return storedPeripherals
    }
    
    func AddToStoredPeripherals(peripheral: CBPeripheral) {
        storedPeripherals.append(peripheral)
<<<<<<< HEAD
        let peripheralAddedToArrayNotification = NotificationCenter.default
        peripheralAddedToArrayNotification.post(name: Notification.Name("StoredPeripheral"), object: nil)
=======
>>>>>>> 65240529d9e4c626ae6b0a8ddd12c12966d50a0f
    }
}
