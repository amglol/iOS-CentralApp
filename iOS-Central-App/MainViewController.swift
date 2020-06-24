//
//  MainViewController.swift
//  iOS-Central-App
//
//  Created by Adrian Galecki on 6/23/20.
//  Copyright © 2020 Adrian Galecki. All rights reserved.
//

import UIKit
import CoreBluetooth

class MainViewController: UIViewController {
    
    @IBOutlet weak var peripheralName: UILabel!
    @IBOutlet weak var peripheralUUID: UILabel!
    @IBOutlet weak var peripheralConnectionUUID: UILabel!
    @IBOutlet weak var peripheralCharUUID: UILabel!
    
    //property labels
    @IBOutlet weak var reakCheck: UIImageView!
    @IBOutlet weak var writeCheck: UIImageView!
    @IBOutlet weak var notifyCheck: UIImageView!
    
    //write to peripheral input field
    @IBOutlet weak var writeValueInputField: UITextField!
    
    //read from peripheral display label
    @IBOutlet weak var readValueDisplayLabel: UILabel!
    
    //notify display labels
    @IBOutlet weak var notifyChar1: UILabel!
    @IBOutlet weak var notifyChar2: UILabel!
    
    //characteristic labels
    @IBOutlet weak var firstCharLabel: UILabel!
    @IBOutlet weak var secondCharLabel: UILabel!
    @IBOutlet weak var thirdCharLabel: UILabel!
    @IBOutlet weak var forthCharLabel: UILabel!
    
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

    var data: CBPeripheral!
    var name: String?
    
    var firstView = ViewController()
    
    override func viewDidLoad() {
        super.viewDidLoad()

        //print(name!)
        
        print(data!)
    }

}
