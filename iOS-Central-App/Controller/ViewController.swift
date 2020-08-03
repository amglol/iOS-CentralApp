//
//  ViewController.swift
//  iOS-Central-App
//
//  Created by Adrian Galecki on 5/18/20.
//  Copyright © 2020 Adrian Galecki. All rights reserved.
//

import UIKit
import CoreBluetooth

class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource, UISceneDelegate, ParentTableViewControllerDelegate {
    
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
    
    //write/read text fields
    @IBOutlet weak var write2345char: UITextField!
    @IBOutlet weak var write2347char: UITextField!
    @IBOutlet weak var read2348char: UILabel!
    
    
    //Connection Constants
    //service id
    let serviceID_A = "1234"
    let serviceID_B = "1235"
    
    //characteristics for service A
    let characteristicID1_A = "2346" //notify
    let characteristicID2_A = "2345" //write - LED Toggle
    
    //characteristics for service B
    let characteristicID1_B = "2347" //write - string
    let characteristicID2_B = "2348" //read - should read whatever was written by 2347
    
//    var centralManager: CBCentralManager!
    var iPadPeripheral: CBPeripheral!
    
    //read data variable
    var readDataFromPeripheral: String = ""
    var writeData: String = ""
    var isSendMsgPressed: Bool = false
    
    var storedPeripherals = [CBPeripheral]()
    var testStorePeripheral = [CBPeripheral]()
    
    //store .write characteristics
    var storedCharacteristics = [CBCharacteristic]()
    
    //store .notify characteristics
    var storedNotifyCharacteristics = [CBCharacteristic]()
    
    var serviceCounter = 1
    var charCounter = 1
    var isDisconnectFlag: Bool = false
    
    var dataModel: FlowmeterData!
    let notification = NotificationCenter.default
    
    let centralManager = BluetoothService.bleServiceInstance.centralManager
    var peripheralManager = BluetoothService.bleServiceInstance.peripheralObject
//    var storedPeripherals = PeripheralManager.peripheralManagerInstance.GetStoredPeripherals()
    var parentTableViewDelegate: ParentTableViewControllerDelegate?

    override func viewDidLoad() {
        super.viewDidLoad()
        
        //assign the table view to the data from the view controller
//        InitTableData()
        
        //assign the central manager
//        centralManager = CBCentralManager(delegate: self, queue: nil)
        
        //gesture to hide keyboard when tapping anywhere on the screen
        SetupTapGesture()
        
        //test for background notification
        let notificationCenter = NotificationCenter.default
        notificationCenter.addObserver(self, selector: #selector(appMovedToBackground), name: UIApplication.willResignActiveNotification, object: nil)
        
//        dataModel = FlowmeterData()
        
        storedPeripherals = PeripheralManager.peripheralManagerInstance.GetStoredPeripherals()
       
    }
    
    func RequestToReloadTableViewData() {
        myTableView.reloadData()
    }
    
    @IBAction func showTranslationsPageAction(_ sender: Any) {
        performSegue(withIdentifier: "showTranslations", sender: self)
    }
    
    @objc func appMovedToBackground() {
        print("App moved to background!")
    }
    
    func CheckForStoredPeripherals() -> Bool {
        var isPeripheralFound = false
        if storedPeripherals.isEmpty {
            isPeripheralFound = false
        }
        else {
            isPeripheralFound = true
        }
        
        return isPeripheralFound
    }
    
    func SetupTapGesture() {
        let tap = UITapGestureRecognizer(target: self.view, action: #selector(UIView.endEditing))
        tap.cancelsTouchesInView = false
        view.addGestureRecognizer(tap)
    }
    
    func InitTableData() {
        self.myTableView.delegate = self
        self.myTableView.dataSource = self
    }
    
    //disconnect from peripheral button event
    @IBAction func disconnectPeripheral(_ sender: Any) {
        print("Disconnecting from peripheral - btn press")
        
        //call the disconnect function
//        CancelConnectionToPeripheral()
        BluetoothService.bleServiceInstance.DisconnectFromPeripheral()
    }
    
    //button event handler
    @IBAction func writeValToChar2345Action(_ sender: Any) {
        let characteristicCount = CheckStoredCharacteristicsArray()
        
        //check if there is a characteristic and then write to it
        if characteristicCount > 0 {
            PrepareMessageForWrite(data: write2345char.text!, characteristic: storedCharacteristics[0])
        }
    }
    
    //button event handler
    @IBAction func writeValToChar2347Action(_ sender: Any) {
//        let characteristicCount = CheckStoredCharacteristicsArray()
//
//        //check if there is a characteristic and then write to it
//        if characteristicCount > 0 {
//            PrepareMessageForWrite(data: write2347char.text!, characteristic: storedCharacteristics[1])
//        }
        Characteristics.characteristicsInstance.AssignDesiredCharacteristic(enterCharacteristicName: "startProcedure")
        
        let startProcedure = Characteristics.characteristicsInstance.GetRequestedCharacteristic()
        
        print("in the WRITE 2347- selected char = \(startProcedure)")
    }
    
    func CheckStoredCharacteristicsArray() -> Int {
        let numberOfCharacteristics = storedCharacteristics.count
        var count = 0
        
        if numberOfCharacteristics > 0 {
            count = numberOfCharacteristics
        }
        else {
            count = 0
        }
        
        return count
    }
    
    func PrepareMessageForWrite(data: String, characteristic: CBCharacteristic) {
        print("Attempting to write to the flowmeter with the value - \(data)")
        
        //convert the message into a Data object
        let convertedData: Data = Data(data.utf8)
     
        //send the data
//        iPadPeripheral.writeValue(convertedData, for: characteristic, type: .withResponse)
        if let peripheralManager = peripheralManager {
            peripheralManager.writeValue(convertedData, for: characteristic, type: .withResponse)
        }
    }
    
    //read data from peripheral button
    @IBAction func readValueBtn(_ sender: Any) {
        print("read btn pressed")
        
        Characteristics.characteristicsInstance.AssignDesiredCharacteristic(enterCharacteristicName: "startProcedure")
        
        let value = Characteristics.characteristicsInstance.GetRequestedCharacteristic()
        
        print("In the READ BTN - selected char = \(value)")
        
        if let peripheralManager = peripheralManager {
            peripheralManager.readValue(for: value)
        }
        
//        if storedCharacteristics.isEmpty {
//            readValueDisplayLabel.text = "null"
//        }
//        else {
////            iPadPeripheral.readValue(for: storedCharacteristics[1])
//            if let peripheralManager = peripheralManager {
//                peripheralManager.readValue(for: storedCharacteristics[1])
//            }
//            readValueDisplayLabel.text = String(readDataFromPeripheral)
//        }
    }
    
    //scan for available peripherals
    func ScanForPeripherals() {
        //scan for a specific peripheral
        centralManager.scanForPeripherals(withServices: nil, options: nil)
    }
    
    //connect to the selected peripheral
    func ConnectToDiscoveredPeripheral() {
        print("CONNECTING TO THIS PERIPHERAL: \(peripheralManager!)")
     
//        centralManager.connect(iPadPeripheral, options: [CBConnectPeripheralOptionNotifyOnConnectionKey: true])
        if let peripheralManager = peripheralManager {
            print("cnct connecting to \(peripheralManager)")
            centralManager.connect(peripheralManager, options: [CBConnectPeripheralOptionNotifyOnConnectionKey: true])
        }
    }
    
//    //system finds out which bluetooth state the central device is in
//    func centralManagerDidUpdateState(_ central: CBCentralManager) {
//        switch central.state {
//        case .unknown:
//            print("bluetooth status is UNKNOWN")
//        case .resetting:
//            print("bluetooth status is RESETTING")
//        case .unsupported:
//            print("bluetooth status is UNSUPPORTED")
//        case .unauthorized:
//            print("bluetooth status is UNAUTHORIZED")
//        case .poweredOn:
//            print("bluetooth status is POWERED ON")
//
//            //scan for the available peripherals that are in range
//            ScanForPeripherals()
//
//        case .poweredOff:
//            print("bluetooth status is POWERED OFF")
//        default:
//            print("status undefined")
//        }
//    }
    
//    //function is called when the peripheral is discovered in the scanning phase
//    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
//        print("DID DISCOVER PERIPHERALS")
//
//        //add peripheral to array only if it has a Name, no duplicates
//        if (peripheral.name != nil && storedPeripherals.contains(peripheral) == false) {
//            //store the discovered peripherals
//            storedPeripherals.append(peripheral)
//        }
//
//        //TABLE VIEW DATA
//        DispatchQueue.main.async {
//            self.myTableView.reloadData()
//        }
//    }
    
//    //central device connected to peripheral
//    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
//        print("Connected to \(peripheral.name!)\n")
//
//        //reset the disconnect flag
//        isDisconnectFlag = false
//
////        iPadPeripheral = peripheral
//        peripheralManager = peripheral
//
//        //display the name of the peripheral
//        DispatchQueue.main.async {
//            self.peripheralName.text = peripheral.name
//        }
//
//        //stop the scan
//        centralManager.stopScan()
//
//        //add iPad as the delegate
////        iPadPeripheral.delegate = self
//        peripheralManager?.delegate = self
//
//        //display the connection id text on screen
////        DispatchQueue.main.async {
////            self.peripheralConnectionUUID.text = "\(self.iPadPeripheral.identifier)"
////        }
//        if let peripheralManager = peripheralManager {
//            DispatchQueue.main.async {
//                self.peripheralConnectionUUID.text = "\(peripheralManager.identifier)"
//            }
//        }
//
//        //proceed to find the peripherals services
//        iPadPeripheral.discoverServices(nil)
//    }
    
//    //loss of connection between central and peripheral
//    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
//        print("DISCONNECTED FROM PERIPHERAL\n")
//
//        //display alert when the devices disconnect
//        ShowAlertWhenBLEDisonnects()
//
//        //attempt to connect to last connected peripheral
//        let connectToLastConnectedPeripheral = CheckForStoredPeripherals()
//        print("\(connectToLastConnectedPeripheral) = state of stored peripherals")
//        if connectToLastConnectedPeripheral {
//            print("Attempting to connect to last known peripherl")
//            ConnectToDiscoveredPeripheral()
//        }
//        else {
//            //scan for peripherals
//            ScanForPeripherals()
//        }
//    }
    
    func CancelConnectionToPeripheral() {
        if !storedNotifyCharacteristics.isEmpty {
            //get the number of stored characteristics
            let count = storedNotifyCharacteristics.count
            
            //unsubscribe from the .notify characteristics
            for num in 1...count {
                iPadPeripheral.setNotifyValue(false, for: storedNotifyCharacteristics[num-1])
            }
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
        
        //set the disconnect flag to true
        isDisconnectFlag = true
    }
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, titleForHeaderInSection section: Int) -> String? {
        return "Periperhals"
    }

    //table view # of items
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        notification.addObserver(self, selector: #selector(PeripheralStoredToArray), name: Notification.Name("StoredPeripheral"), object: nil)
        
        print("xcr - testStorePeripheral count = \(testStorePeripheral.count)")
        
        return testStorePeripheral.count
    
    }
    
    //a row was pressed
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        //let currentCell = tableView.cellForRow(at: indexPath)! as UITableViewCell

        let myAlert = UIAlertController(title: "Selected Row", message: "You selected row: \(indexPath.row)", preferredStyle: .alert)
        
        //assign the selected peripheral to a variable
        peripheralManager = testStorePeripheral[indexPath.row]

        //add the connect to device button in the alert
        myAlert.addAction(UIAlertAction(title: "Connect", style: .default) {_ in self.ConnectToDiscoveredPeripheral()})
        myAlert.addAction(UIAlertAction(title: "Dismiss", style: .default))

        self.present(myAlert, animated: true, completion: nil)
    }
    
    //display data in a tableview
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
//        let cell = myTableView.dequeueReusableCell(withIdentifier: "peripheralList", for: indexPath)
//
//
//        let peripheral = storedPeripherals[indexPath.row]
//        let uuid = peripheral.identifier
//        cell.textLabel?.text = peripheral.name!
//        cell.textLabel?.textColor = UIColor.red
//        cell.detailTextLabel?.text = "\(uuid)"
//        cell.detailTextLabel?.textColor = UIColor.black
        if let cell = tableView.dequeueReusableCell(withIdentifier: "peripheralList", for: indexPath) as? PeripheralCell {
            print("Stored peripherals count in the table view cell xcr = \(testStorePeripheral.count)")
            let peripheralCell = testStorePeripheral[indexPath.row]
            
            print("sending this peripheral to updateCell s1 - \(testStorePeripheral[0])")
            
            cell.UpdateCellView(peripheral: peripheralCell)
            //cell.parentTableViewDelegate = self
            return cell
        }
        else {
            return PeripheralCell()
        }
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if let translationVC = segue.destination as? TranslationsVC {
            translationVC.dataModel = dataModel
        }
    }
    
    @objc func PeripheralStoredToArray() {
        print("xcr - notifiction hit")
        testStorePeripheral = PeripheralManager.peripheralManagerInstance.GetStoredPeripherals()
        InitTableData()
        myTableView.reloadData()
    }
    
}

//extension ViewController: CBPeripheralDelegate {
//    //find out what services the peripheral has
//    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
//        print("DID DISCOVER SERVICES METHOD")
//        guard let services = iPadPeripheral.services else { return }
//
//        //convert the string ID's to CBUUID's
//        let customServiceCBUUID = CBUUID(string: self.serviceID_A)
//        let customServiceCBUUID1 = CBUUID(string: self.serviceID_B)
//
//        //we only care about services that match our service ID
//        for service in services {
//
//            print("\nService #\(serviceCounter)- \(service) \n")
//            serviceCounter += 1
//
//            switch service.uuid {
//            case customServiceCBUUID:
//                print("Found characteristic properties for the service -> \(service.uuid)")
//                //now find the characteristics for that service
//                peripheral.discoverCharacteristics(nil, for: service)
//            case customServiceCBUUID1:
//                print("Found characteristic properties for the service -> \(service.uuid)")
//                //now find the characteristics for that service
//                peripheral.discoverCharacteristics(nil, for: service)
//            default:
//                print("Did not find characteristic properties for -> \(service)")
//            }
//        }
//    }
//
//    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor descriptor: CBDescriptor, error: Error?) {
//        print("DID UPDATE VALUE FOR DESCRIPTOR")
//    }
//
//    //find out the characters for the peripherals service
//    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
//        print("\nDID DISCOVER CHARACTERISTICS FOR")
//        guard let characteristics = service.characteristics else { return }
//
//        print("Found \(characteristics.count) characteristics\n")
//        let serviceChar1_A = CBUUID(string: characteristicID1_A)
//        let serviceChar2_A = CBUUID(string: characteristicID2_A)
//        let serviceChar1_B = CBUUID(string: characteristicID1_B)
//        let serviceChar2_B = CBUUID(string: characteristicID2_B)
//
//        for characteristic in characteristics {
//            print("Characteristic #\(charCounter) - \(characteristic)")
//            charCounter += 1
//
//            //update the display text
//            DispatchQueue.main.async {
//                self.peripheralUUID.text = "\(self.serviceID_A) / \(self.serviceID_B)"
//                self.peripheralCharUUID.text = "\(self.characteristicID1_A) / \(self.characteristicID1_B)"
//            }
//
//            if characteristic.properties.contains(.read) {
//                print("the service == \(service) == has these characterstics:")
//                print("\(characteristic.uuid) contains .read")
//                iPadPeripheral.readValue(for: characteristic)
//
//                //update the ui to show the checkmark
//                DispatchQueueUI(myView: self.readCheck)
//            }
//
//            if characteristic.properties.contains(.write) {
//                print("\(characteristic.uuid) contains .write\n")
//
//                if (!storedCharacteristics.contains(characteristic)) {
//                    //store the characteristics
//                    storedCharacteristics.append(characteristic)
//                }
//                //update the ui to show the checkmark
//                DispatchQueueUI(myView: self.writeCheck)
//            }
//
//            if characteristic.properties.contains(.notify) {
//              print("\(characteristic.uuid): properties contains .notify")
//
//                //store the characteristic
//                storedNotifyCharacteristics.append(characteristic)
//
//              //update the ui to show the checkmark
//              DispatchQueueUI(myView: self.notifyCheck)
//
//              switch characteristic.uuid {
//              case serviceChar1_A:
//                  iPadPeripheral.setNotifyValue(true, for: characteristic)
//                  print("set notify property for \(serviceChar1_A)")
//
//              case serviceChar2_A:
//                  iPadPeripheral.setNotifyValue(true, for: characteristic)
//                  print("set notify property for \(serviceChar2_A)")
//
//              case serviceChar1_B:
//                  iPadPeripheral.setNotifyValue(true, for: characteristic)
//                  print("set notify property for \(serviceChar1_B)")
//              case serviceChar2_B:
//                iPadPeripheral.setNotifyValue(true, for: characteristic)
//                print("set notify property for \(serviceChar2_B)")
//              default:
//                  break
//              }
//            }
//
//            if characteristic.properties.contains(.indicate) {
//                print("\(characteristic.uuid): properties contain .indicate")
//
//                //update the ui to show the checkmark
//                DispatchQueueUI(myView: self.indicateCheck)
//            }
//
//            if characteristic.properties.contains(.writeWithoutResponse) {
//                print("\(characteristic.uuid): properties contain .writeWithoutResponse")
//
//                //update the ui to show the checkmark
//                DispatchQueueUI(myView: self.writeWithoutRspCheck)
//            }
//        }
//    }
//
//    func DispatchQueueUI(myView: UIView) {
//
//        if (isDisconnectFlag == false) {
//            DispatchQueue.main.async {
//                myView.isHidden = false
//            }
//        }
//        else {
//            DispatchQueue.main.async {
//                myView.isHidden = true
//            }
//        }
//    }
//
//    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
//        print("\n=== DID UPDATE VALUE FOR ===")
//
//        let serviceChar1_A = CBUUID(string: characteristicID1_A)
//        let serviceChar2_A = CBUUID(string: characteristicID2_A)
//        let serviceChar1_B = CBUUID(string: characteristicID1_B)
//        let serviceChar2_B = CBUUID(string: characteristicID2_B)
//
//        switch characteristic.uuid {
//        case serviceChar1_A:
//            print("FIRST CHAR")
//            print("Value for \(serviceChar1_A): ", characteristic.value ?? "No Value Found For \(serviceChar1_A)")
//
//            let readValue = characteristic.value ?? "no value".data(using: .utf8)
//            print("ReadValue in firstChar before its converted = \(readValue)")
//
//            let converted = readValue!.hexEncodedString()
//
//            print("converted value = \(converted)")
//
//            //assign the message to data model
//            dataModel.incomingMsg = converted
//
//            //readDataFromPeripheral = converted
//            self.firstCharValLabel.text = converted
//
//            guard let msg = dataModel.incomingMsg else { return }
//            NotificationCenter.default.post(name: Notification.Name("IncomingMsg"), object: msg, userInfo: nil)
//
//
//        case serviceChar2_A:
//            print("SECOND CHAR")
//            print("Value for \(serviceChar2_A): ", characteristic.value ?? "No Value Found For \(serviceChar2_A)")
//
//            let readValue = characteristic.value ?? "no value".data(using: .utf8)
//            print("ReadValue in secondChar before its converted = \(readValue)")
//
//            let converted = readValue!.hexEncodedString()
//
//            print("converted value = \(converted)")
//
//            self.secondCharValLabel.text = converted
//        case serviceChar1_B:
//            print("THIRD CHAR")
//            print("Value for \(serviceChar1_B): ", characteristic.value ?? "No Value Found For \(serviceChar1_B)")
//
//            let readValue = characteristic.value ?? "no value".data(using: .utf8)
//            print("ReadValue in thirdChar before its converted = \(readValue)")
//
//            let converted = readValue!.hexEncodedString()
//
//            print("DATA READ from thirdChar = \(converted)")
//            readDataFromPeripheral = converted
//            self.thirdCharValLabel.text = converted
//        case serviceChar2_B:
//            print("FORTH CHAR")
//            print("Value for \(serviceChar2_B): ", characteristic.value ?? "No Value Found For \(serviceChar2_B)")
//
//
//            let readValue = characteristic.value ?? "no value".data(using: .utf8)
//            print("ReadValue in forthChar before its converted = \(readValue)")
//
//            let converted = readValue!.hexEncodedString()
//
//            //pass the value from this read to global variable
//            readDataFromPeripheral = converted
//
//            print("DATA READ from forthChar = \(converted)")
//        default:
//            print("ERR: could not read characteristic")
//        }
//    }
//
//    func peripheral(_ peripheral: CBPeripheral, didWriteValueFor characteristic: CBCharacteristic, error: Error?) {
//        print("DID WRITE VALUE FOR METHOD")
//    }
//
//    func peripheral(_ peripheral: CBPeripheral, didModifyServices invalidatedServices: [CBService]) {
//        print("SERVICES MODIFIED")
//        ScanForPeripherals()
//    }
//
//    func peripheral(_ peripheral: CBPeripheral, didUpdateNotificationStateFor characteristic: CBCharacteristic, error: Error?) {
//        print("\(characteristic.uuid) state was changed to \(characteristic.isNotifying)")
//    }
//}
//
////convert data to string format
//extension Data {
// struct HexEncodingOptions: OptionSet {
//     let rawValue: Int
//     static let upperCase = HexEncodingOptions(rawValue: 1 << 0)
// }
// func hexEncodedString(options: HexEncodingOptions = []) -> String {
//     let format = options.contains(.upperCase) ? "%02hhX" : "%02hhx"
//     return map { String(format: format, $0) }.joined()
// }
//}



