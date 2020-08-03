//
//  ServicesHandler.swift
//  iOS-Central-App
//
//  Created by Adrian Galecki on 7/28/20.
//  Copyright © 2020 Adrian Galecki. All rights reserved.
//

import Foundation

struct ServicesModel {
    private(set) public var serviceID: String
    
    init(serviceID: String) {
        self.serviceID = serviceID
    }
}

struct Services {
    static let servicesInstance = Services()
    
    private let services = [
        ServicesModel(serviceID: "1234"),
        ServicesModel(serviceID: "1235")
    ]
    
    func GetServiceIDs() -> [ServicesModel] {
        return services
    }
}
