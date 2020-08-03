//
//  TranslationsVC.swift
//  iOS-Central-App
//
//  Created by Adrian Galecki on 7/6/20.
//  Copyright © 2020 Adrian Galecki. All rights reserved.
//

import UIKit
import LanguageManager_iOS

class TranslationsVC: UIViewController {
    
    var dataModel: FlowmeterData!
    let languages = ELanguages()
    let defaults = UserDefaults.standard
    
    @IBOutlet weak var toggleDropdownBtn: UIButton!
    @IBOutlet weak var tableView: UITableView!
    
    @IBOutlet weak var notifyCharLbl: UILabel!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        print("IN THE TRANSLATION VC")
        
        tableView.dataSource = self
        tableView.delegate = self
        
        tableView.isHidden = true
        
        NotificationCenter.default.addObserver(self, selector: #selector(UpdateTest(notification:)), name: Notification.Name("IncomingMsg"), object: nil)
        
        

    }
    
    @objc func UpdateTest(notification: Notification) {
        print("Value has been updated")
        if let msg = notification.object {
            print(msg)
            FlowmeterData.flowmeterDataInstance.testData = "\(msg)"
            
            //pass the incoming msg number to get translated
            GetCorrectTranslation(notificationObject: msg)
        }
    }
    
    func GetCorrectTranslation(notificationObject msg: Any) {
        print("In the GetCorrectTranslation Func")
        guard let convertedMsgToInt = Int((msg as? String)!) else {return}
        
        print("ConvertedMsgToInt = \(convertedMsgToInt)")
        
        if convertedMsgToInt % 2 == 0 {
            print("NUMBER is EVEN")
            //number is even
            notifyCharLbl.text = NSLocalizedString("2", comment: "world msg")
        } else {
            print("NUMBER is ODD")
            //number is odd
            notifyCharLbl.text = NSLocalizedString("1", comment: "greeting msg")
        }
    }
    
    @IBAction func toggleDropdownBtnTapped(_ sender: Any) {
        if tableView.isHidden {
            AnimateDropdown(toggle: true, passInTableView: tableView)
        }
        else {
            AnimateDropdown(toggle: false, passInTableView: tableView)
        }
    }
}

extension TranslationsVC: UITableViewDelegate, UITableViewDataSource {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return languages.languages.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "languages", for: indexPath)
        
        cell.textLabel?.text = languages.languages[indexPath.row]
        
        return cell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let selectedLang = languages.languages[indexPath.row]
        var currentLanguage: Languages
        
        switch selectedLang {
        case "English":
            print("selected english")
            currentLanguage = .en
            
            // change the language
            LanguageManager.shared.setLanguage(language: currentLanguage,
                                               viewControllerFactory: { title -> UIViewController in
              let storyboard = UIStoryboard(name: "Main", bundle: nil)
              // the view controller that you want to show after changing the language
              return storyboard.instantiateInitialViewController()!
            }) { view in
              // do custom animation
              view.transform = CGAffineTransform(scaleX: 2, y: 2)
              view.alpha = 0
            }
            break
        case "Spanish":
            print("selected spanish")
            currentLanguage = .es
            
            // change the language
            LanguageManager.shared.setLanguage(language: currentLanguage,
                                               viewControllerFactory: { title -> UIViewController in
              let storyboard = UIStoryboard(name: "Main", bundle: nil)
              // the view controller that you want to show after changing the language
              return storyboard.instantiateInitialViewController()!
            }) { view in
              // do custom animation
              view.transform = CGAffineTransform(scaleX: 2, y: 2)
              view.alpha = 0
            }
            break
        default:
            currentLanguage = .en
            break
        }
    }
    
    
}
