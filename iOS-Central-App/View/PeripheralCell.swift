//
//  PeripheralCell.swift
//  iOS-Central-App
//
//  Created by Adrian Galecki on 7/28/20.
//  Copyright © 2020 Adrian Galecki. All rights reserved.
//

import UIKit
import CoreBluetooth

class PeripheralCell: UITableViewCell {
    
    var parentTableViewDelegate: ParentTableViewControllerDelegate?

    @IBOutlet weak var peripheralName: UILabel!
    @IBOutlet weak var uuidNumber: UILabel!
    
    func UpdateCellView(peripheral: CBPeripheral) {
        peripheralName.text = peripheral.name
        uuidNumber.text = "\(peripheral.identifier)"
        
        //reload table view data
        parentTableViewDelegate?.RequestToReloadTableViewData()
    }

}
