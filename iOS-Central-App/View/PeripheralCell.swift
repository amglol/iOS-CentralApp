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
<<<<<<< HEAD
        print("xcr1 peripheral in the cell class = \(peripheral)")
=======
>>>>>>> 65240529d9e4c626ae6b0a8ddd12c12966d50a0f
        peripheralName.text = peripheral.name
        uuidNumber.text = "\(peripheral.identifier)"
        
        //reload table view data
<<<<<<< HEAD
//        parentTableViewDelegate?.RequestToReloadTableViewData()
=======
        parentTableViewDelegate?.RequestToReloadTableViewData()
>>>>>>> 65240529d9e4c626ae6b0a8ddd12c12966d50a0f
    }

}
