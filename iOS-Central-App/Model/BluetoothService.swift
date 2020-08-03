//
//  BluetoothService.swift
//  iOS-Central-App
//
//  Created by Adrian Galecki on 7/21/20.
//  Copyright © 2020 Adrian Galecki. All rights reserved.
//

import Foundation
import CoreBluetooth

class BluetoothService: NSObject, CBCentralManagerDelegate, CBPeripheralDelegate {
    
    static let bleServiceInstance = BluetoothService()
    private(set) var centralManager = CentralManager.centralManagerInstance.GetCbCentralManagerInstance()
    private(set) var peripheralObject: CBPeripheral!
    private(set) var parentTableViewDelegate: ParentTableViewControllerDelegate?
    
    override init() {
        super.init()
        self.centralManager = CBCentralManager(delegate: self, queue: nil)
    }
    
    func StartScan() {
        self.centralManager.scanForPeripherals(withServices: nil, options: nil)
    }
    
    func StopScan() {
        self.centralManager.stopScan()
    }
    
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        switch central.state {
        case .unknown:
            print("BLE status: unknown")
        case .resetting:
            print("BLE status: resetting")
        case .unsupported:
            print("BLE status: unsupported")
        case .unauthorized:
            print("BLE status: unauthorized")
        case .poweredOn:
            StartScan()
        case .poweredOff:
            print("BLE status: powered off")
        default:
            print("BLE status: undefined")
        }
    }
    
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        print("DID DISCOVER PERIPHERALS")
        
        //check if peripheral name is equal to nil or not
        if let flowmeterName = peripheral.name {
            print("discovered flowmeter: \(flowmeterName)")
            
            if !PeripheralManager.peripheralManagerInstance.GetStoredPeripherals().contains(peripheral) {
                print("ADDING PERIPHERAL TO ARRAY \(peripheral)")
                //add the peripheral to the storedPeripheral array
                PeripheralManager.peripheralManagerInstance.AddToStoredPeripherals(peripheral: peripheral)
            }
            
            print("StoredPeripherals count = \(PeripheralManager.peripheralManagerInstance.GetStoredPeripherals())")
            
            //reload table view data
            parentTableViewDelegate?.RequestToReloadTableViewData()
        }
    }
    
    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        
        //stop the scan
        StopScan()
    }
    
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        print("DID CONNECT TO PERIPHERAL")
        
        //assign the peripheral to the peripheral object
        peripheralObject = peripheral
        
        //assign the delegate
        peripheralObject.delegate = self
        
        //stop the scan
        StopScan()
        
        //discover that peripherals services
        peripheralObject.discoverServices(nil)
    }
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        print("DID DISCOVER SERVICES METHOD")
        
        guard let services = peripheralObject.services else { return }
        
        print("Services : \(services)")
    }
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        print("DID DISCOVER CHARACTERISTICS FOR")
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor descriptor: CBDescriptor, error: Error?) {
        print("DID UPDATE VALUE FOR")
    }
    
    func peripheral(_ peripheral: CBPeripheral, didWriteValueFor characteristic: CBCharacteristic, error: Error?) {
        print("DID WRITE VALUE FOR METHOD")
    }
    
    func peripheral(_ peripheral: CBPeripheral, didModifyServices invalidatedServices: [CBService]) {
        print("SERVICES MODIFIED")
        StartScan()
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateNotificationStateFor characteristic: CBCharacteristic, error: Error?) {
        print("\(characteristic.uuid) state was changed to \(characteristic.isNotifying)")
    }
    
    
}
