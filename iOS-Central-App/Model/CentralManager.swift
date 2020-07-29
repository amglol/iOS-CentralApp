//
//  CentralManager.swift
//  iOS-Central-App
//
//  Created by Adrian Galecki on 7/16/20.
//  Copyright © 2020 Adrian Galecki. All rights reserved.
//

import UIKit
import CoreBluetooth

class CentralManager: CBCentralManager {
    
    private let cbCentralManager = CBCentralManager()
    static let centralManagerInstance = CentralManager()
    
    //return an instance of CBCentralManager
    func GetCbCentralManagerInstance() -> CBCentralManager {
        return cbCentralManager
    }
}
