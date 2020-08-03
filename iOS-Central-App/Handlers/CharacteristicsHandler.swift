//
//  CharacteristicsHandler.swift
//  iOS-Central-App
//
//  Created by Adrian Galecki on 7/28/20.
//  Copyright © 2020 Adrian Galecki. All rights reserved.
//

import Foundation
import CoreBluetooth

struct CharacteristicsModel {
    private(set) public var characteristicID: String
    
    init(characteristicID: String) {
        self.characteristicID = characteristicID
    }
}

class Characteristics {
    //******************************************************************
    //VARIABLES
    static let characteristicsInstance = Characteristics()
    var storedCharacteristics = [CBCharacteristic]()
    private(set) var requestedCharacteristic: CBCharacteristic!
    //******************************************************************
    
    //create the dictionary using the CharacteristicsModel
    private let characteristics = [
        "oxygenValue": CharacteristicsModel(characteristicID: "2345"),
        "nitrousValue": CharacteristicsModel(characteristicID: "2346"),
        "startProcedure": CharacteristicsModel(characteristicID: "2347"),
        "testProcedure": CharacteristicsModel(characteristicID: "2348"),
        "flush": CharacteristicsModel(characteristicID: "2349")
    ]
    
    //retreives the characteristic model dictionary
    func GetCharacteristcsIDs() -> [String: CharacteristicsModel] {
        return characteristics
    }
    
    //add the peripherals characteristics into an array
    func AddCharacteristicToArray(characteristic: CBCharacteristic) {
        storedCharacteristics.append(characteristic)
    }
    
    //retreive the peripherals characteristics
    func GetStoredCharacteristics() -> [CBCharacteristic] {
        return storedCharacteristics
    }
    
    //retreive the characteristic thats being requested by the user
    func GetRequestedCharacteristic() -> CBCharacteristic {
        return requestedCharacteristic
    }
    
    //takes the name of the requested characteristic and filters through the peripherals characteristics to find a match
    func AssignDesiredCharacteristic(enterCharacteristicName charName: String) {
        
        //verify that the provided characeristic name exists in the dictionary
        guard let characteristicName = GetCharacteristcsIDs()[charName] else { return }
        
        //convert the string id into a CBUUID
        let retrievedCharacteristicCBUUID = CBUUID(string: characteristicName.characteristicID)
        
        //loop through peripheral characeristics until a match is found
        for i in 0..<Characteristics.characteristicsInstance.GetStoredCharacteristics().count {
            if Characteristics.characteristicsInstance.GetStoredCharacteristics()[i].uuid == retrievedCharacteristicCBUUID {
                
                //assign the characteristic thats being requested
                requestedCharacteristic = Characteristics.characteristicsInstance.GetStoredCharacteristics()[i]
            }
        }
        
    }
}
