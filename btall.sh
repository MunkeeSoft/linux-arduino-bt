#!/bin/sh
/home/garrett/bt/bt.py -k aNsLE_OKrwloZgX12kUNLKQXHoaSAKxKU1l2YWk4SlRHWT0g -f 77741 -d Temperature 07:12:05:16:70:12 temp
sleep 5
/home/garrett/bt/bt.py -k aNsLE_OKrwloZgX12kUNLKQXHoaSAKxKU1l2YWk4SlRHWT0g -f 77741 -d Humidity 07:12:05:16:70:12 humid
sleep 5
/home/garrett/bt/bt.py -k aNsLE_OKrwloZgX12kUNLKQXHoaSAKxKU1l2YWk4SlRHWT0g -f 77741 -d Light 07:12:05:16:70:12 light
