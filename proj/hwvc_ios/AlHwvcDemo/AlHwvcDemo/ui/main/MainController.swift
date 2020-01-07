//
//  ViewController.swift
//  AlHwvcDemo
//
//  Created by lmy on 2020/1/5.
//  Copyright © 2020 Alimin. All rights reserved.
//

import UIKit

class MainController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    
    @IBOutlet weak var mTableView: UITableView!
    var items: Array<String> = ["Video", "Image"]
    override func viewDidLoad() {
        super.viewDidLoad()
        mTableView.dataSource = self;
        mTableView.delegate = self;
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return items.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cellid = "cellID"
        var cell = tableView.dequeueReusableCell(withIdentifier: cellid)
        if nil == cell {
            cell = UITableViewCell(style: .subtitle, reuseIdentifier: cellid)
        }
        cell!.textLabel?.text = self.items[indexPath.row]
        return cell!
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        switch indexPath.row {
        case 0:
            break
        case 1:
            let controller = ImageController()
            present(controller, animated: true)
            break
        default:
            break
        }
    }
}

