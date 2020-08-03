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
    
    //****************************************************
    //VARIABLES
    
    //create an instance of Bluetooth Service
    static let bleServiceInstance = BluetoothService()
    //fetch the instance of central manager and assign to variable
    private(set) var centralManager = CentralManager.centralManagerInstance.GetCbCentralManagerInstance()
    private(set) var peripheralObject: CBPeripheral!
    private(set) var parentTableViewDelegate: ParentTableViewControllerDelegate?
    private(set) var serviceHandler = Services.servicesInstance.GetServiceIDs()
    private(set) var characteristicHandler = Characteristics.characteristicsInstance.GetCharacteristcsIDs()
    
    //****************************************************
    
    override init() {
        super.init()
        //initialize the central manager as the delegate
        self.centralManager = CBCentralManager(delegate: self, queue: nil)
    }
    
    func StartScan() {
        //scan for every peripheral
        self.centralManager.scanForPeripherals(withServices: nil, options: nil)
    }
    
    func DisconnectFromPeripheral() {
        self.centralManager.cancelPeripheralConnection(peripheralObject)
    }
    
    func StopScan() {
        //stop scan
        self.centralManager.stopScan()
    }
    
    func ConnectBackToPeripheral() {
        if let peripheral = peripheralObject {
            centralManager.connect(peripheral, options: [CBConnectPeripheralOptionNotifyOnConnectionKey: true])
        }
    }
    
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        //check which state the central (ios app) is in
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
            
            //check if the peripheral has already been added to the peripheral array. We don't want duplicates
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
        
        if !PeripheralManager.peripheralManagerInstance.GetStoredPeripherals().isEmpty {
            ConnectBackToPeripheral()
        }
        else {
            //stop the scan
            StartScan()
        }
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
    
    //*************************************
    //PERIPHERAL MANAGER PROTOCOL FUNCTIONS
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        print("DID DISCOVER SERVICES METHOD")
        
        guard let services = peripheralObject.services else { return }
        
        print("Services : \(services)")
        
        for service in services {
            for serviceId in serviceHandler {
                let id = CBUUID(string: serviceId.serviceID)
                switch service.uuid {
                case id:
                    print("Found characteristic properties for the service -> \(service.uuid)")
                    peripheralObject.discoverCharacteristics(nil, for: service)
                default:
                    print("Did not find any characteristics")
                }
            }
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        print("DID DISCOVER CHARACTERISTICS FOR")
        guard let characteristics = service.characteristics else { return }
        //print("chr = \(characteristics)")
        for characteristic in characteristics {
            //store the characeristics into an array
            Characteristics.characteristicsInstance.AddCharacteristicToArray(characteristic: characteristic)
            
            if characteristic.properties.contains(.read) {
                print("chr Found read char - \(characteristic)")
                peripheralObject.readValue(for: characteristic)
            }
            if characteristic.properties.contains(.notify) {
                print("chr Found notify char - \(characteristic)")
                peripheralObject.setNotifyValue(true, for: characteristic)
                print("chr - SETTING notify=true for \(characteristic)")
            }
            if characteristic.properties.contains(.write) {
                print("chr Found write char")
            }
        }
        
        print("chr Characteristic count = \(Characteristics.characteristicsInstance.GetStoredCharacteristics().count)")
        print("chr Stored Chars = \(Characteristics.characteristicsInstance.GetStoredCharacteristics())")
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor descriptor: CBDescriptor, error: Error?) {
        print("DID UPDATE VALUE FOR DESCRIPTOR")
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        print("== DID UPDATE VALUE FOR CHARACTERISTIC ==")
        
        guard let incomingData = characteristic.value else { return }
        
        let nitrous = get2346()
        let flush = get2349()
        let startP = get2347()
        
        switch characteristic.uuid {
        case nitrous.uuid:
            print("chr - Incoming message from peripherals characteristic uuid \(characteristic.uuid) Value = \(incomingData)")
        case flush.uuid:
            print("chr - Incoming message from peripherals characteristic uuid \(characteristic.uuid) Value = \(incomingData)")
        case startP.uuid:
            print("chr - Incoming message from peripherals characteristic uuid \(characteristic.uuid) Value = \(incomingData)")
        default:
            print("NO VALUE")
        }
    }
    
    func get2346() -> CBCharacteristic {
        Characteristics.characteristicsInstance.AssignDesiredCharacteristic(enterCharacteristicName: "nitrousValue")
        let nitrous = Characteristics.characteristicsInstance.GetRequestedCharacteristic()
        
        return nitrous
        
    }
    
    func get2347() -> CBCharacteristic {
        Characteristics.characteristicsInstance.AssignDesiredCharacteristic(enterCharacteristicName: "startProcedure")
        let startP = Characteristics.characteristicsInstance.GetRequestedCharacteristic()
        
        return startP
        
    }
    
    func get2349() -> CBCharacteristic {
        Characteristics.characteristicsInstance.AssignDesiredCharacteristic(enterCharacteristicName: "flush")
        let flush = Characteristics.characteristicsInstance.GetRequestedCharacteristic()
        
        return flush
        
    }
    
    func updateValue(_ value: Data,
                 for characteristic: CBMutableCharacteristic,
                 onSubscribedCentrals centrals: [CBCentral]?) -> Bool {
        print("updated characteristic: \(characteristic)")
        return true
    }
    
    func peripheral(_ peripheral: CBPeripheral, didWriteValueFor characteristic: CBCharacteristic, error: Error?) {
        print("DID WRITE VALUE FOR METHOD")
    }
    
    func peripheral(_ peripheral: CBPeripheral, didModifyServices invalidatedServices: [CBService]) {
        print("SERVICES MODIFIED")
        StartScan()
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateNotificationStateFor characteristic: CBCharacteristic, error: Error?) {
        print("chr == \(characteristic.uuid) state was changed to \(characteristic.isNotifying)")
    }
    
    
    
}
