//
//  HelperFunctions.swift
//  iOS-Central-App
//
//  Created by Adrian Galecki on 7/6/20.
//  Copyright © 2020 Adrian Galecki. All rights reserved.
//

import UIKit

extension UIViewController {
    //display an alert message
    func ShowAlertWhenBLEDisonnects() {
        let alertController = UIAlertController(title: "BLE Signal", message: "Lost Signal - Disconnected", preferredStyle: .alert)
        
        alertController.addAction(UIAlertAction(title: "Dismiss", style: .default))
        
        self.present(alertController, animated: true, completion: nil)
    }
    
    func AnimateDropdown(toggle: Bool, passInTableView tblView: UITableView) {
        if toggle {
            UIView.animate(withDuration: 0.3) {
                tblView.isHidden = false
            }
        }
        else {
            UIView.animate(withDuration: 0.3) {
                tblView.isHidden = true
            }
        }
    }
}

protocol ParentTableViewControllerDelegate {
    func RequestToReloadTableViewData()
}
