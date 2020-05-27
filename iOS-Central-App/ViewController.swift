//
//  ViewController.swift
//  iOS-Central-App
//
//  Created by Adrian Galecki on 5/18/20.
//  Copyright © 2020 Adrian Galecki. All rights reserved.
//

import UIKit
import CoreBluetooth

class ViewController: UIViewController, CBCentralManagerDelegate, UITableViewDelegate, UITableViewDataSource {
    
    @IBOutlet weak var peripheralReadValueBtn: UIButton!
    @IBOutlet weak var peripheralReadValueLabel: UILabel!
    @IBOutlet weak var myTextView: UILabel!
    @IBOutlet weak var myTextField: UITextField!
    @IBOutlet weak var peripheralName: UILabel!
    @IBOutlet weak var peripheralUUID: UILabel!
    @IBOutlet weak var peripheralConnectionUUID: UILabel!
    @IBOutlet weak var peripheralCharUUID: UILabel!
    @IBOutlet weak var readCheck: UIImageView!
    @IBOutlet weak var writeCheck: UIImageView!
    @IBOutlet weak var notifyCheck: UIImageView!
    @IBOutlet weak var indicateCheck: UIImageView!
    @IBOutlet weak var writeWithoutRspCheck: UIImageView!
    @IBOutlet weak var myTableView: UITableView!
    @IBOutlet weak var valueSendBtn: UIButton!
    
    //connection id
    let connectionID = "1010"
    //service id
    let serviceID = "2233"
    //characteristic id
    let characteristicID = "2169"
    //second characteristic id
    let characteristicID2 = "1212"
    
    var centralManager: CBCentralManager!
    var iPadPeripheral: CBPeripheral!
    
    //read data variable
    var readDataFromPeripheral: String = ""
    var writeData: String = ""
    var isSendMsgPressed: Bool = false
    
    //store discovered peripherals
    var storedPeripherals = [String: UUID]()
    var RSSIs = [NSNumber]()

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view
        
        //assign the table view to the data from the view controller
        self.myTableView.delegate = self
        self.myTableView.dataSource = self

        //assign the central manager
        centralManager = CBCentralManager(delegate: self, queue: nil)
        
        //gesture to hide keyboard when tapping anywhere on the screen
        let tap = UITapGestureRecognizer(target: self.view, action: #selector(UIView.endEditing))
        view.addGestureRecognizer(tap)
    }
    
    //Button event handler
    //send user input to peripheral device
    @IBAction func valueChanged(_ sender: Any) {
        print("value is changing")
        
//        iPadPeripheral.writeValue(data!, for: characteristic)
    }
    
    //Button event handler
    //send user input to peripheral device
    @IBAction func sendValueBtn(_ sender: Any) {
        isSendMsgPressed = true
        writeData = myTextField.text!
    }
    
    //Button event handler
    //read peripheral data
    @IBAction func ReadValueFromPeripheral(_ sender: Any) {
        print("Read Value BTN pressed")
        peripheralReadValueLabel.text = String(readDataFromPeripheral)
    }
    
    //scan for available peripherals
    func ScanForPeripherals() {
        //convert to CBUUID value
        let serviceCBUUID = CBUUID(string: self.connectionID)
        
        //scan for a specific peripheral
        centralManager.scanForPeripherals(withServices: nil, options: nil)
    }
    
    //connect to the selected peripheral
    func ConnectToDiscoveredPeripheral() {
        //connect to the discovered peripheral
        centralManager.connect(iPadPeripheral, options: nil)
    }
    
    //system finds out which bluetooth state the central device is in
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        switch central.state {
        case .unknown:
            print("bluetooth status is UNKNOWN")
        case .resetting:
            print("bluetooth status is RESETTING")
        case .unsupported:
            print("bluetooth status is UNSUPPORTED")
        case .unauthorized:
            print("bluetooth status is UNAUTHORIZED")
        case .poweredOn:
            print("bluetooth status is POWERED ON")
            
            //scan for the available peripherals that are in range
            ScanForPeripherals()
            
        case .poweredOff:
            print("bluetooth status is POWERED OFF")
        default:
            print("status undefined")
        }
    }
    
    //function is called when the peripheral is discovered in the scanning phase
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        print("DID DISCOVER PERIPHERALS")
        
        print(peripheral)
        
        //display the name of the peripheral
        DispatchQueue.main.async {
            self.peripheralName.text = peripheral.name
        }
        
//        print(peripheral)
//        
//        if ((peripheral.name) != nil)
//        {
//            print("in the IF statement")
//            let peripheralNameExists = storedPeripherals[peripheral.name!] != nil
//            if (!peripheralNameExists)
//            {
//                self.storedPeripherals[peripheral.name!] = peripheral.identifier
//            }
//        }
//        else {
//            print("in the ELSE block")
//            let name = "null" + String(count)
//            let peripheralIdentifierExists = storedPeripherals.values.contains(peripheral.identifier)
//            
//            if(!peripheralIdentifierExists) {
//                print("identifier was not found- ADD NEW ONE")
//                self.storedPeripherals[name] = peripheral.identifier
//            }
//            count += 1
//        }
        
        //TABLE VIEW DATA
//        DispatchQueue.main.async {
//            self.myTableView.reloadData()
//        }
        
        //add the RSSI value
//        self.RSSIs.append(RSSI)
        
        //stop the scan
//        centralManager.stopScan()

        //store the peripheral once it is found
        iPadPeripheral = peripheral

        //add iPad as the delegate
        iPadPeripheral.delegate = self

        //connect to the ipad
//        ConnectToDiscoveredPeripheral()
        
    }
    
    //central device connected to peripheral
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        print("Connected to iPad\n")
        
        //display the connection id text on screen
        DispatchQueue.main.async {
            self.peripheralConnectionUUID.text = self.connectionID
        }
        
        //proceed to find the peripherals services
        iPadPeripheral.discoverServices(nil)
    }
    
    //loss of connection between central and peripheral
    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        print("DISCONNECTED FROM PERIPHERAL\n")
        
        //reconnect back to peripheral
        ConnectToDiscoveredPeripheral()
    }
    
    //table view # of items
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        //print("STORED PERIPHERALS COUNT = \(storedPeripherals.count)")
        return storedPeripherals.count
    }
    
    //a row was pressed
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let currentCell = tableView.cellForRow(at: indexPath)! as UITableViewCell

        print(currentCell.textLabel!.text!)
    }
    
    //display data in a tableview
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = myTableView.dequeueReusableCell(withIdentifier: "peripheralList", for: indexPath)
        
        let keysArray = Array(storedPeripherals.keys)
    
        cell.textLabel?.text = keysArray[indexPath.row]
//        for peripheral in storedPeripherals {
//            cell.textLabel?.text = peripheral.key
//        }
        return cell
    }
}

extension ViewController: CBPeripheralDelegate {
    
    //find out what services the peripheral has
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        print("DID DISCOVER SERVICES METHOD")
        guard let services = peripheral.services else { return }
        
        //convert the string ID's to CBUUID's
        let customServiceCBUUID = CBUUID(string: self.serviceID)
        
        print("\n \(services) \n")
        
        //we only care about services that match our service ID
        for service in services {
            if service.uuid == customServiceCBUUID {
                print("Found the read, write, notify service on the \(service.uuid)")
                //now find the characteristics for that service
                peripheral.discoverCharacteristics(nil, for: service)
            }
            else {
                print("Did not find characteristic for \(service)")
            }
            
        }
    }
    
    //find out the characters for the peripherals service
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        print("\nDID DISCOVER CHARACTERISTICS FOR")
        guard let characteristics = service.characteristics else { return }
        
        for characteristic in characteristics {
            print(characteristic)
            
            //update the display text
            DispatchQueue.main.async {
                self.peripheralUUID.text = self.serviceID
                self.peripheralCharUUID.text = self.characteristicID
            }

            if characteristic.properties.contains(.read) {
                print("the service == \(service) == has these characterstics:")
                print("\(characteristic.uuid) contains .read")
                peripheral.readValue(for: characteristic)
                
                //update the ui to show the checkmark
                DispatchQueueUI(myView: self.readCheck)
            }

            if characteristic.properties.contains(.write) {
                print("\(characteristic.uuid) contains .write")
                
                //update the ui to show the checkmark
                DispatchQueueUI(myView: self.writeCheck)
            }

            if characteristic.properties.contains(.notify) {
              print("\(characteristic.uuid): properties contains .notify")
              peripheral.setNotifyValue(true, for: characteristic)
                
                //update the ui to show the checkmark
                DispatchQueueUI(myView: self.notifyCheck)
            }
            
            if characteristic.properties.contains(.indicate) {
                print("\(characteristic.uuid): properties contain .indicate")
                
                //update the ui to show the checkmark
                DispatchQueueUI(myView: self.indicateCheck)
            }
            
            if characteristic.properties.contains(.writeWithoutResponse) {
                print("\(characteristic.uuid): properties contain .writeWithoutResponse")
                
                //update the ui to show the checkmark
                DispatchQueueUI(myView: self.writeWithoutRspCheck)
            }
        }
    }
    
    func DispatchQueueUI(myView: UIView) {
        DispatchQueue.main.async {
            myView.isHidden = false
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        print("\n=== DID UPDATE VALUE FOR ===")
        print("Read Request from peripheral device")
        
        //print("Characteristic read: \(characteristic)\n error: \(error)")
        let firstChar = CBUUID(string: characteristicID)
        let secondChar = CBUUID(string: characteristicID2)
        
        print("FirstChar = \(firstChar) //// SecondChar = \(secondChar)")
        
        switch characteristic.uuid {
        case firstChar:
            print("Value for \(firstChar): ", characteristic.value ?? "No Value Found for \(firstChar)")
            print("before calling readData func, characteristic value = \(characteristic)")
            
            //convert the data into string format to display on the screen
            
            if (characteristic.value != nil)
            {
                let readValue = characteristic.value!
//                let readValue = characteristic.value![0]
                
                //assign the converted text to a display label
//                readDataFromPeripheral = readValue
                
                let converted = String.init(data: readValue, encoding: .utf8)
                
                readDataFromPeripheral = converted!
                
                print("DATA READ = \(converted ?? "no value")")
            }

        case secondChar:
            print("Value for \(secondChar): ", characteristic.value ?? "No Value Found for \(secondChar)")
            //convert the data into string format to display on the screen
            let readValue = characteristic.value!.hexEncodedString()
            
            //assign the converted text to a display label
            readDataFromPeripheral = readValue
            print("DATA READ = \(readValue)")
        default:
            print("ERR: could not read characteristic")
        }
        
        if (isSendMsgPressed) {
            print("writing to peripheral with \(writeData)")
            let data: Data = writeData.data(using: .utf8)!
            iPadPeripheral.writeValue(data, for: characteristic, type: .withResponse)
            isSendMsgPressed = false
        }
        
        
        //write to peripheral
        
//        let data = Data(bytes: bytes, count: 2)
//
//        peripheral.writeValue(data, for: characteristic, type: .withoutResponse)
    }
    
    func peripheral(_ peripheral: CBPeripheral, didWriteValueFor characteristic: CBCharacteristic, error: Error?) {
        print("DID WRITE VALUE FOR METHOD")
    }
    
    func peripheral(_ peripheral: CBPeripheral, didModifyServices invalidatedServices: [CBService]) {
        print("SERVICES MODIFIED")
        ScanForPeripherals()
    }
}

//convert data to string format
extension Data {
 struct HexEncodingOptions: OptionSet {
     let rawValue: Int
     static let upperCase = HexEncodingOptions(rawValue: 1 << 0)
 }
 func hexEncodedString(options: HexEncodingOptions = []) -> String {
     let format = options.contains(.upperCase) ? "%02hhX" : "%02hhx"
     return map { String(format: format, $0) }.joined()
 }
}



