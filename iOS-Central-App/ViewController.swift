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
    
    @IBOutlet weak var peripheralName: UILabel!
    @IBOutlet weak var peripheralUUID: UILabel!
    @IBOutlet weak var peripheralConnectionUUID: UILabel!
    @IBOutlet weak var peripheralCharUUID: UILabel!
    
    //properties labels
    @IBOutlet weak var readCheck: UIImageView!
    @IBOutlet weak var writeCheck: UIImageView!
    @IBOutlet weak var notifyCheck: UIImageView!
    @IBOutlet weak var indicateCheck: UIImageView!
    @IBOutlet weak var writeWithoutRspCheck: UIImageView!
    
    @IBOutlet weak var myTableView: UITableView!
    
    //write to peripheral input field
    @IBOutlet weak var writeValueInputField: UITextField!
    
    //read from peripheral display label
    @IBOutlet weak var readValueDisplayLabel: UILabel!
    
    //first characteristic display label
    @IBOutlet weak var firstCharLabel: UILabel!
    
    @IBOutlet weak var firstCharValLabel: UILabel!
    @IBOutlet weak var secondCharValLabel: UILabel!
    @IBOutlet weak var thirdCharValLabel: UILabel!
    
    
    //connection id
//    let connectionID = "1010"
    let connectionID = "0101"
    
    //service id
    let serviceID = "2233"
//    let serviceID = "0101"
    let serviceID1 = "6969"
    
    //characteristic id
    let characteristicID = "2169"
    //let characteristicID = "1111" //read write notify
    //second characteristic id
    let characteristicID2 = "1212"
    //let characteristicID2 = "2222" //read
    //char id
    let characteristicID3 = "1333"
//    let characteristicID3 = "3333" //read notify
    //forth char id
    let characteristicID4 = "6999"
    
    
    var centralManager: CBCentralManager!
    var iPadPeripheral: CBPeripheral!
    
    //read data variable
    var readDataFromPeripheral: String = ""
    var writeData: String = ""
    var isSendMsgPressed: Bool = false
    
    var storedPeripherals = [CBPeripheral]()
    var RSSIs = [NSNumber]()
    
    //store .write characteristics
    var storedCharacteristics = [CBCharacteristic]()
    
    //store .notify characteristics
    var storedNotifyCharacteristics = [CBCharacteristic]()
    
    var serviceCounter = 1
    var charCounter = 1
    var isDisconnectFlag: Bool = false

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
        tap.cancelsTouchesInView = false
        view.addGestureRecognizer(tap)
    }
    
    //disconnect from peripheral button event
    @IBAction func disconnectPeripheral(_ sender: Any) {
        print("Disconnecting from peripheral - btn press")
        
        //call the disconnect function
        CancelConnectionToPeripheral()
    }
    
    //write data to peripheral button
    @IBAction func writeValueBtn(_ sender: Any) {
        print("write btn pressed")
        
        let arrayCount = storedCharacteristics.count
        
        //verify that there is a write characteristic property
        if (arrayCount > 0) {
            isSendMsgPressed = true
            writeData = writeValueInputField.text!
            print(writeValueInputField.text!)
            
            print("writing to peripheral with \(writeData)")
            let data: Data = Data(writeData.utf8)
            let writeCharacteristic = storedCharacteristics[0]
            
            //write to the peripheral
            iPadPeripheral.writeValue(data, for: writeCharacteristic, type: .withResponse)
        }
        else {
            let msg = "The service you are subscribed to does not have a .write property"
            //notify the user that there is no write characteristic property for the service
            let alert = UIAlertController(title: "Warning", message: msg, preferredStyle: .alert)
            alert.addAction(UIAlertAction(title: "Dismiss", style: .default))
            
            //present the alert
            self.present(alert, animated: true, completion: nil)
        }
    }
    
    //read data from peripheral button
    @IBAction func readValueBtn(_ sender: Any) {
        print("read btn pressed")
        readValueDisplayLabel.text = String(readDataFromPeripheral)
        
        for char in storedCharacteristics {
            let notifying = char.isNotifying
            print("Char: \(char) has the notifying property enabled: \(notifying)")
        }
    }
    
    //scan for available peripherals
    func ScanForPeripherals() {
        //convert to CBUUID value
        //let serviceCBUUID = CBUUID(string: self.connectionID)
        
        //scan for a specific peripheral
        centralManager.scanForPeripherals(withServices: nil, options: nil)
    }
    
    //connect to the selected peripheral
    func ConnectToDiscoveredPeripheral() {
        print("CONNECTING TO THIS PERIPHERAL: \(iPadPeripheral!)")
        //connect to the discovered peripheral
        centralManager.connect(iPadPeripheral, options: [CBConnectPeripheralOptionNotifyOnConnectionKey: true])
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
    
        //add peripheral to array only if it has a Name, no duplicates
        if (peripheral.name != nil && storedPeripherals.contains(peripheral) == false) {
            //store the discovered peripherals
            storedPeripherals.append(peripheral)
        }
        
        //TABLE VIEW DATA
        DispatchQueue.main.async {
            self.myTableView.reloadData()
        }
    }
    
    //central device connected to peripheral
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        print("Connected to \(peripheral.name!)\n")
        
        //display the name of the peripheral
        DispatchQueue.main.async {
            self.peripheralName.text = peripheral.name
        }
        
        //stop the scan
        centralManager.stopScan()
        
        //add iPad as the delegate
        iPadPeripheral.delegate = self
        
        //display the connection id text on screen
        DispatchQueue.main.async {
//            self.peripheralConnectionUUID.text = self.connectionID
            self.peripheralConnectionUUID.text = "\(self.iPadPeripheral.identifier)"
        }
        //proceed to find the peripherals services
        peripheral.discoverServices(nil)
    }
    
    //loss of connection between central and peripheral
    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        print("DISCONNECTED FROM PERIPHERAL\n")
        
        //display alert when the devices disconnect
        ShowAlertWhenBLEDisonnects()
        
        //reconnect back to peripheral
        ScanForPeripherals()
    }
    
    func CancelConnectionToPeripheral() {
        //get the number of stored characteristics
        let count = storedNotifyCharacteristics.count
        
        print("COUNT = \(count)")
        
        //unsubscribe from the .notify characteristics
        for num in 1...count {
            iPadPeripheral.setNotifyValue(false, for: storedNotifyCharacteristics[num-1])
        }
        
        //cancel the connection
        centralManager.cancelPeripheralConnection(iPadPeripheral)
        
        //clear the text labels / fields on the display
        peripheralName.text = ""
        peripheralConnectionUUID.text = ""
        peripheralUUID.text = ""
        peripheralCharUUID.text = ""
        firstCharValLabel.text = ""
        secondCharValLabel.text = ""
        thirdCharValLabel.text = ""
    }
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, titleForHeaderInSection section: Int) -> String? {
        return "Periperhals"
    }

    //table view # of items
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        print("STORED PERIPHERALS COUNT = \(storedPeripherals.count)")
        return storedPeripherals.count
    }
    
    //a row was pressed
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        //let currentCell = tableView.cellForRow(at: indexPath)! as UITableViewCell

        let myAlert = UIAlertController(title: "Selected Row", message: "You selected row: \(indexPath.row)", preferredStyle: .alert)
        
        print("Selected PERIPHERAL IS = \(storedPeripherals[indexPath.row])")
        
        //assign the selected peripheral to a variable
        iPadPeripheral = storedPeripherals[indexPath.row]
        
        //add the connect to device button in the alert
        myAlert.addAction(UIAlertAction(title: "Connect", style: .default) {_ in self.ConnectToDiscoveredPeripheral()})
        myAlert.addAction(UIAlertAction(title: "Dismiss", style: .default))
        
        self.present(myAlert, animated: true, completion: nil)
    }
    
    //display data in a tableview
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = myTableView.dequeueReusableCell(withIdentifier: "peripheralList", for: indexPath)
        
        
        let peripheral = storedPeripherals[indexPath.row]
        let uuid = peripheral.identifier
        cell.textLabel?.text = peripheral.name!
        cell.textLabel?.textColor = UIColor.red
        cell.detailTextLabel?.text = "\(uuid)"
        cell.detailTextLabel?.textColor = UIColor.black
        return cell
    }
    
    //display an alert message
    func ShowAlertWhenBLEDisonnects() {
        let alertController = UIAlertController(title: "BLE Signal", message: "Lost Signal - Disconnected", preferredStyle: .alert)
        
        alertController.addAction(UIAlertAction(title: "Dismiss", style: .default))
        
        self.present(alertController, animated: true, completion: nil)
    }
}

extension ViewController: CBPeripheralDelegate {
    //find out what services the peripheral has
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        print("DID DISCOVER SERVICES METHOD")
        guard let services = iPadPeripheral.services else { return }
        
        //convert the string ID's to CBUUID's
        let customServiceCBUUID = CBUUID(string: self.serviceID)
        let customServiceCBUUID1 = CBUUID(string: self.serviceID1)
        
        //we only care about services that match our service ID
        for service in services {
            
            print("\nService #\(serviceCounter)- \(service) \n")
            serviceCounter += 1
            
            switch service.uuid {
            case customServiceCBUUID:
                print("Found characteristic properties for the service -> \(service.uuid)")
                //now find the characteristics for that service
                peripheral.discoverCharacteristics(nil, for: service)
            case customServiceCBUUID1:
                print("Found characteristic properties for the service -> \(service.uuid)")
                //now find the characteristics for that service
                peripheral.discoverCharacteristics(nil, for: service)
            default:
                print("Did not find characteristic properties for -> \(service)")
            }
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor descriptor: CBDescriptor, error: Error?) {
        print("DID UPDATE VALUE FOR DESCRIPTOR")
    }
    
    //find out the characters for the peripherals service
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        print("\nDID DISCOVER CHARACTERISTICS FOR")
        guard let characteristics = service.characteristics else { return }
        
        print("Found \(characteristics.count) characteristics\n")
        let char1 = CBUUID(string: characteristicID)
        let char2 = CBUUID(string: characteristicID2)
        let char3 = CBUUID(string: characteristicID3)
        
        for characteristic in characteristics {
            print("Characteristic #\(charCounter) - \(characteristic)")
            charCounter += 1
            
            //update the display text
            DispatchQueue.main.async {
                self.peripheralUUID.text = self.serviceID
                self.peripheralCharUUID.text = "\(self.characteristicID2) / \(self.characteristicID3)"
            }
            
            if characteristic.properties.contains(.read) {
                print("the service == \(service) == has these characterstics:")
                print("\(characteristic.uuid) contains .read")
                iPadPeripheral.readValue(for: characteristic)
                
                //update the ui to show the checkmark
                DispatchQueueUI(myView: self.readCheck)
            }

            if characteristic.properties.contains(.write) {
                print("\(characteristic.uuid) contains .write\n")
                
                if (!storedCharacteristics.contains(characteristic)) {
                    //store the characteristics
                    storedCharacteristics.append(characteristic)
                }
                //update the ui to show the checkmark
                DispatchQueueUI(myView: self.writeCheck)
            }

            if characteristic.properties.contains(.notify) {
              print("\(characteristic.uuid): properties contains .notify")
                
                //store the characteristic
                storedNotifyCharacteristics.append(characteristic)

              //update the ui to show the checkmark
              DispatchQueueUI(myView: self.notifyCheck)
              
              switch characteristic.uuid {
              case char1:
                  iPadPeripheral.setNotifyValue(true, for: characteristic)
                  print("set notify property for \(char1)")
                  
              case char2:
                  iPadPeripheral.setNotifyValue(true, for: characteristic)
                  print("set notify property for \(char2)")
                
              case char3:
                  iPadPeripheral.setNotifyValue(true, for: characteristic as CBCharacteristic)
                  print("set notify property for \(char3)")
              default:
                  break
              }
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
        
        if (isDisconnectFlag == false) {
            DispatchQueue.main.async {
                myView.isHidden = false
            }
        }
        else {
            DispatchQueue.main.async {
                myView.isHidden = true
            }
        }
        
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        print("\n=== DID UPDATE VALUE FOR ===")
        
        let firstChar = CBUUID(string: characteristicID)
        let secondChar = CBUUID(string: characteristicID2)
        let thirdChar = CBUUID(string: characteristicID3)
        let forthChar = CBUUID(string: characteristicID4)
        
        //print("Characteristic \(characteristic.uuid) value = \(characteristic.value!)")
        
        switch characteristic.uuid {
        case firstChar:
            print("FIRST CHAR")
            print("Value for \(firstChar): ", characteristic.value ?? "No Value Found For \(firstChar)")
            
            let readValue = characteristic.value ?? "no value".data(using: .utf8)
            print("ReadValue in firstChar before its converted = \(readValue)")

            let converted = readValue!.hexEncodedString()

            print("converted value = \(converted)")

            //readDataFromPeripheral = converted
            self.firstCharValLabel.text = converted

        case secondChar:
            print("SECOND CHAR")
            print("Value for \(secondChar): ", characteristic.value ?? "No Value Found For \(secondChar)")
            
            let readValue = characteristic.value!
            print("ReadValue in secondChar before its converted = \(readValue)")

            let converted = readValue.hexEncodedString()
            
            print("converted value = \(converted)")

            readDataFromPeripheral = converted
            
            self.secondCharValLabel.text = converted
        case thirdChar:
            print("THIRD CHAR")
            print("Value for \(thirdChar): ", characteristic.value ?? "No Value Found For \(thirdChar)")
            
            let readValue = characteristic.value!
            print("ReadValue in thirdChar before its converted = \(readValue)")

            let converted = readValue.hexEncodedString()

            readDataFromPeripheral = converted

            print("DATA READ from thirdChar = \(converted)")
            
            self.thirdCharValLabel.text = converted
        case forthChar:
            print("FORTH CHAR")
            print("Value for \(forthChar): ", characteristic.value ?? "No Value Found For \(forthChar)")
            
            let readValue = characteristic.value!
            print("ReadValue in forthChar before its converted = \(readValue)")
                        
            let converted = readValue.hexEncodedString()
                        
            readDataFromPeripheral = converted
                        
            print("DATA READ from thirdChar = \(converted)")
        default:
            print("ERR: could not read characteristic")
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didWriteValueFor characteristic: CBCharacteristic, error: Error?) {
        print("DID WRITE VALUE FOR METHOD")
        let myChar = CBUUID(string: characteristicID2)
        if (characteristic.uuid == myChar) {
            peripheral.readValue(for: storedCharacteristics[0])
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didModifyServices invalidatedServices: [CBService]) {
        print("SERVICES MODIFIED")
        ScanForPeripherals()
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateNotificationStateFor characteristic: CBCharacteristic, error: Error?) {
        print("\(characteristic.uuid) state was changed to \(characteristic.isNotifying)")
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



