import React from "react";
import { useState, useMemo } from "react";
import { Search, Cpu, Smartphone, Filter, ChevronDown, ChevronRight, User, Users, TestTube } from "lucide-react";
import { Input } from "@/components/ui/input";
import { Badge } from "@/components/ui/badge";
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";
import { Collapsible, CollapsibleContent, CollapsibleTrigger } from "@/components/ui/collapsible";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import { Table, TableBody, TableCell, TableHead, TableHeader, TableRow } from "@/components/ui/table";

interface Device {
  name: string;
  brand: string;
  status: 'working' | 'partial' | 'not-working' | 'unknown' | 'active' | 'inactive';
  codename?: string;
  maintainer?: string;
  contributors?: string[];
  testers?: string[];
  uefiFeatures?: UEFIFeature[];
  windowsFeatures?: WindowsFeature[];
}

interface UEFIFeature {
  feature: string;
  description?: string;
  state: 'working' | 'not-working' | 'partial' | 'unknown';
}

interface WindowsFeature {
  feature: string;
  description?: string;
  state: 'working' | 'not-working' | 'partial' | 'unknown';
}

interface Chipset {
  name: string;
  devices: Device[];
}

interface Manufacturer {
  name: string;
  icon: string;
  chipsets: Chipset[];
}

const manufacturers: Manufacturer[] = [
  {
    name: "Qualcomm",
    icon: "🔥",
    chipsets: [
      {
        name: "Snapdragon 8 Elite",
        devices: [
          { 
            name: "Oneplus 13", 
            brand: "Oneplus", 
            status: "active",
            codename: "dodge",
            maintainer: "index986",
            contributors: ["Robotix", "index986", "N1kroks"],
            testers: ["index986"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "Windows Boot", description: "Untested", state: "unknown" },
              { feature: "Linux Boot", description: "Mainline soon WIP", state: "partial" },
              ],
          },
        ],
      },
      
      //------------------------------------------------------------------------new soc
      
      {
        name: "Snapdragon 8s Gen 3",
        devices: [
          { 
            name: "Realme GT Neo 6", 
            brand: "Realme", 
            status: "active",
            codename: "bale",
            maintainer: "InternalHellhound",
            contributors: [" Robotix", "Shandorman", "InternalHellhound", "index986"," Daniel224455", "Aistop"],
            testers: ["InternalHellhound", "@slyqfs (Telegram)"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "Windows Boot", description: "WinPE Booted", state: "working" },
              { feature: "Linux Boot", description: "Mainline WIP", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "not-working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
           },
//------------------------------------------------------------------------
          { 
          name: "Xiaomi Poco F6", brand: "Xiaomi",
          codename: "peridot",
            maintainer: "PugzAreCute",
            status: "active",
            contributors: [" Robotix, PugzAreCute, InternalHellhound, iNUCi, Daniel224455, Aistop"],
            testers: ["PugzAreCute"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", description: "WinPE Booted", state: "working" },
              { feature: "Linux Boot", description: "Mainline WIP", state: "not-working" },
            ],
             windowsFeatures: [
              { feature: "Internal Storage", state: "not-working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
          },
      ],
    },
    
      //------------------------------------------------------------------------new soc
      
      {
        name: "Snapdragon 8 Gen 2",
        devices: [
          { name: "OnePlus 12R / Ace 3", brand: "Oneplus", status: "inactive",
          maintainer: "Shandorman",
          codename: "aston",
            contributors: [" Robotix, Shandorman"],
            testers: ["Shandorman"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", description: "Only Battery", state: "partial" },
              { feature: "Battery", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "unknown" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", description: "No Indication", state: "partial" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
          },
          //------------------------------------------------------------------------
          { 
          name: "ZTE Nubia RedMagic 8 Pro", brand: "ZTE", status: "active",
          maintainer: "Nikka",
          codename: "nx729j",
            contributors: ["Robotix, Nikka"],
            testers: ["Nikka"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "working" },
              { feature: "Battery", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "working" },
              { feature: "WLAN", state: "working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "working" },
              { feature: "Bluetooth", state: "working" },
              { feature: "GPS", state: "working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "working" },
              ],
          },
        ],
      },
      //------------------------------------------------------------------------
      {
        name: "Snapdragon 8 Gen 1/8+ Gen 1 Devices",
        devices: [
          { 
          name: "Samsung Galaxy Tab S8+ 5G", brand: "Samsung", status: "active",
          codename: "gts8p",
          maintainer: "Robotix",
            contributors: ["Robotix"],
            testers: ["Robotix"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", description: "Requires a Reconnect if it was Connected during Boot", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "SD Card", description: "Power On Code is Missing", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Gyroscope Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              { feature: "S-pen", state: "not-working" },
              ],
          },
          //------------------------------------------------------------------------
          { 
          name: "Samsung Galaxy Tab S8 5G", brand: "Samsung", status: "inactive",
          maintainer: "None",
          codename: "gts8",
            contributors: ["Robotix"],
            testers: ["Robotix"],
            uefiFeatures: [
              { feature: "Display", description: "Wrong display orentation", state: "partial" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", description: "Power button does not work", state: "partial" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", description: "Needs Special Setup", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Gyroscope Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", description: "One Core only", state: "partial" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", description: "Wrong orientation", state: "partial" },
              { feature: "Vibration", state: "not-working" },
              { feature: "S-pen", state: "not-working" },
              ],
          },
          //------------------------------------------------------------------------
          { 
          name: "Samsung Galaxy Z Flip 4", brand: "Samsung", status: "active",
          codename: "b4q",
          maintainer: "Robotix",
            contributors: ["Robotix"],
            testers: ["Robotix"],
            uefiFeatures: [
              { feature: "Main Display", state: "working" },
              { feature: "Cover Display", description: "No actual Driver to Handle it & Wrong Orientation", state: "partial" },
              { feature: "Internal Storage", description:"Doesn't Init Properly sometimes", state: "partial" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", description:"Requires a Reconnect if it was Connected during Boot", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
            { feature: "Internal Storage", description: "Needs Special Setup", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Gyroscope Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "partial" },
              { feature: "Vibration", state: "not-working" },
              ],
          },
          //------------------------------------------------------------------------
          { 
          name: "Xiaomi 12 Pro", brand: "Xiaomi", status: "inactive",
	codename: "zeus",
          maintainer: "None",
            contributors: ["Robotix, 2petro"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
             // { feature: "Cover Display", description: "No actual Driver to Handle it & Wrong Orientation", state: "partial" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "unknown" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
         },
         //------------------------------------------------------------------------
          { 
          name: "Xiaomi Poco F4 GT", brand: "Xiaomi", status: "inactive",
	codename: "ingres",
          maintainer: "Nikka",
            contributors: ["Robotix, Nikka"],
            testers: ["Nikka"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
             // { feature: "Cover Display", description: "No actual Driver to Handle it & Wrong Orientation", state: "partial" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
             windowsFeatures: [
            { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Gyroscope Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              //{ feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
            //------------------------------------------------------------------------
          { 
          name: "Nothing Phone (2)", brand: "Nothing", status: "active",
	codename: "pong",
          maintainer: "index986",
            contributors: ["Robotix, N1kroks, index986"],
            testers: ["index986, People from tg/discord nothing group"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "working" },
            ],
             windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Gyroscope Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
        ],
      },
      
      //------------------------------------------------------------------------new soc
      
      {
        name: "Snapdragon 888/888+ Devices",
        devices: [
          { 
          name: "Samsung Galaxy S21 FE", brand: "Samsung", status: "inactive",
          codename: "r9qb2",
          maintainer: "None",
            contributors: ["Robotix,Nikroks, Icesito68"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", description:"Needs Special Setup", state: "working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", description:"Needs special setup", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
          },
          //------------------------------------------------------------------------
          { 
          name: "Samsung Galaxy Z Fold 3 5G", brand: "Samsung", status: "inactive",
          codename:"q2q",
          maintainer: "Azkali",
            contributors: ["Robotix, Azkali"],
            testers: ["Azkali"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", description: "Power Button does not work", state: "partial" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "unknown" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", description:"Needs special setup", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
          },
          //------------------------------------------------------------------------
          { 
          name: "Oneplus 9", brand: "Oneplus", status: "active", 
          maintainer: "arminask",
            contributors: ["Robotix"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "working" },
              { feature: "Battery", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
          },
          //------------------------------------------------------------------------
          { 
          name: "Asus ROG Phone 5", brand: "Asus", status: "inactive", 
          codename: "i005d",
          maintainer: "None",
            contributors: ["Robotix, alfaonyt"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "not-working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
          },
          //------------------------------------------------------------------------
          { 
          name: "Xiaomi Mi 11", brand: "Xiaomi", status: "inactive", 
          maintainer: "Daniel224455",
            contributors: ["Robotix, Daniel224455"],
            testers: ["Daniel224455"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
           ],
          },
          //------------------------------------------------------------------------
          { 
          name: "Xiaomi 11T Pro", brand: "Xiaomi", status: "active",
          codename: "vili",
          maintainer: "Robotix",
            contributors: ["Robotix"],
            testers: ["Robotix"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
              windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
          },
        ],
      },
      
      //------------------------------------------------------------------------new soc
    {  
      name: "Snapdragon 865/865+/870 Devices",
        devices: [
          { 
          name: "OnePlus 8T", brand: "OnePlus", status: "inactive", maintainer: "Maintainer",
          codename: "kebab",
            contributors: ["Robotix, SwedMlite"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
             { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
            //------------------------------------------------------------------------
          { 
          name: "Lenovo Legion Tab Y700", brand: "Lenovo", status: "inactive", maintainer: "None",
          codename: "9707f",
            contributors: ["hyusang"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
           windowsFeatures: [
             { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
            //------------------------------------------------------------------------
          { 
          name: "Realme GT NEO 2", brand: "Realme", status: "inactive", maintainer: "kubawis128",
            contributors: ["Robotix, kubawis128"],
            testers: ["kubawis128"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "working" },
            ],
             windowsFeatures: [
             { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
            //------------------------------------------------------------------------
          { 
          name: "Xiaomi Pad 6", brand: "Xiaomi", status: "inactive", maintainer: "None",
          codename: "pipa",
            contributors: ["Statzar, N1kroks, Robotix, 6adp"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "working" },
              { feature: "Mass Storage", description: "Unknown issues", state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "working" },
              { feature: "Temperature Sensor", state: "working" },
              { feature: "Battery", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              ],
            },
            //------------------------------------------------------------------------
          { 
          name: "Xiaomi Poco F3 / Redmi K40 / Mi 11X", brand: "Xiaomi", status: "active", maintainer: "N1kroks",
          codename: "alioth",
            contributors: ["Robotix, SwedMlite, hyusang, AdrianoA3, N1kroks"],
            testers: ["N1kroks"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
           },
        ],
      },
      
      //----------------------------------------------------------------------------------------------new soc
      
      
      {
        name: "Snapdragon 855/855+/860 Devices",
        devices: [
          { 
            name: "LG G8s", brand: "LG", status: "inactive", maintainer: "Daniel224455, n00b69",
            contributors: ["Daniel224455, n00b69, Icesito68"],
            testers: ["n00b69"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage",  state: "not-working" },
              { feature: "SD Card", description: "Broken",state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", description:"Untested",state: "unknown" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", description:"Partially works, Disabled for now.", state: "partial" },
              { feature: "Gyroscope Sensor", description:"Partially works, Disabled for now.", state: "partial" },
              { feature: "Accelerometer Sensor",description:"Partially works, Disabled for now.",  state: "partial" },
              { feature: "Compass Sensor",description:"Partially works, Disabled for now.",  state: "partial" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "unknown" },
              { feature: "Battery", state: "working" },
              { feature: "USB", state: "working" },
              { feature: "Charging",description:"USB Charging is slow, Wireless Charging is somewhat Fast.", state: "working" },
              { feature: "WLAN", state: "working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "working" },
              { feature: "Bluetooth", state: "working" },
              { feature: "GPS", state: "working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "3.5mm Audio Jack", state: "working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
            ],
          },
          { name: "OnePlus 7T Pro", brand: "Oneplus", status: "inactive", maintainer: "None",
          codename: "hotdog",
            contributors: ["None"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage",  state: "unknown" },
              //{ feature: "SD Card", state: "working" },
              { feature: "Windows Boot",description: "Windows PE only", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "not-working" },
              //{ feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
             },
          { 
          name: "Xiaomi Pad 5", brand: "Xiaomi", status: "active", maintainer: "iNUCi",
            contributors: ["iNUCi, Robotix"],
            testers: ["iNUCi,"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              //{ feature: "SD Card", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", state: "not-working" },
              //{ feature: "Light Sensor", state: "working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "unknown" },
              { feature: "Battery", state: "working" },
              { feature: "USB", state: "working" },
              { feature: "Charging", description: "Very Slow",state: "partial" },
              { feature: "WLAN", state: "working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "working" },
              { feature: "Bluetooth", state: "working" },
              { feature: "GPS", state: "working" },
              { feature: "Speakers", state: "working" },
              { feature: "Microphone", state: "working" },
              { feature: "GPU", state: "working" },
              { feature: "Camera", state: "not-working" },
              //{ feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
             },
          { 
          name: "Xiaomi Mi 9", brand: "Xiaomi", status: "inactive", maintainer: "Daniel224455",
            contributors: ["Daniel224455"],
            testers: ["motylek"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "not-working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", description: "untested" , state: "unknown" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", state: "not-working" },
              //{ feature: "Light Sensor", state: "working" },
              { feature: "Accelerometer Sensor", state: "working" },
              { feature: "Compass Sensor", state: "working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              //{ feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "unknown" },
              { feature: "Battery", state: "working" },
              { feature: "USB", state: "working" },
              { feature: "Charging",description: "Very slow", state: "partial" },
              { feature: "WLAN", state: "working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "working" },
              { feature: "Bluetooth", state: "working" },
              { feature: "GPS", state: "working" },
              { feature: "Speakers", state: "working" },
              { feature: "Microphone", state: "working" },
              { feature: "GPU", state: "working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
          { 
            name: "Xiaomi Poco X3 Pro", brand: "Xiaomi", status: "inactive", maintainer: "Daniel224455, remtrik",
            contributors: ["Daniel224455, remtrik, Nikroks"],
            testers: ["Daniel224455"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Broken", state: "not-working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", description: "untested", state: "unknown" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", description: "Hot Plug doesn't work", state: "partial" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", state: "not-working" },
              //{ feature: "Light Sensor", state: "working" },
              { feature: "Accelerometer Sensor", state: "working" },
              { feature: "Compass Sensor", state: "working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              //{ feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "unknown" },
              { feature: "Battery", state: "working" },
              { feature: "USB", state: "working" },
              { feature: "Charging", description: "Very Slow", state: "partial" },
              { feature: "WLAN", state: "working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "working" },
              { feature: "Bluetooth", state: "working" },
              { feature: "GPS", state: "working" },
              { feature: "3.5mm Audio Jack", state: "working" },
              { feature: "Speakers", state: "working" },
              { feature: "Microphone", state: "working" },
               { feature: "Flashlight", description: "Accessible only from Windows Camera, acts like a Stroboscope", state: "partial" },
              { feature: "GPU", state: "working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
        ],
      },
      {
        name: "Snapdragon 845 Devices",
        devices: [
          { name: "LG Velvet 4G", brand: "LG", status: "inactive", maintainer: "None",
	  codename: "caymanslm",
            contributors: ["CodeLindro"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "unknown" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "3.5mm Audio Jack" , state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
             },
          { 
          name: "Xiaomi Mi 8 Pro", brand: "Xiaomi", status: "inactive", maintainer: "None",
	codename: "equuleus",
            contributors: ["index986"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              //{ feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
          },
          { name: "Xiaomi Mi Mix 2S", brand: "Xiaomi", status: "inactive", maintainer: "None",
          codename: "polaris",
            contributors: ["Daniel224455, TrustedFloppa"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              //{ feature: "SD Card", state: "working" },
              { feature: "Windows Boot", description: "Windows PE only", state: "partial" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "not-working" },
              //{ feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
             },
        ],
      },
      {
        name: "Snapdragon 835 Devices",
        devices: [
          { name: "Sony Xperia XZ1", brand: "Sony", status: "inactive", maintainer: "None",
            codename: "poplar",
            contributors: ["Robotix"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", description: "Disabled to prevent Wipe", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", description: "Dies on Disconnect", state: "partial" },
              { feature: "Mass Storage", description: "Disabled to prevent Wipe", state: "not-working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "unknown" },
              { feature: "SD Card", state: "working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", description:"Dies on Disconnect", state: "partial" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "3.5mm Audio Jack", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
        ],
      },
      {
        name: "Snapdragon 778G/778G+/782G Devices",
        devices: [
          { name: "Nothing Phone (1)", brand: "Nothing", status: "inactive", maintainer: "None",
          codename: "spacewar",
            contributors: ["Robotix", "index986"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              //{ feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
           },
          { name: "Samsung Galaxy A52s 5G", brand: "Samsung", status: "active", maintainer: "arminask",
            contributors: ["Robotix", "arminask", "People from Group"],
            testers: ["arminask"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", state: "working" },
              { feature: "Light Sensor", state: "working" },
              { feature: "Accelerometer Sensor", state: "working" },
              { feature: "Compass Sensor", state: "working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "working" },
              { feature: "Battery", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "working" },
              { feature: "WLAN", state: "working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "working" },
              { feature: "Bluetooth", state: "working" },
              { feature: "GPS", state: "working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "working" },
              ],
            },
          { 
          name: "Xiaomi 11 Lite 5G NE / Xiaomi 11 Lite NE 5G / Xiaomi Mi 11 LE / Xiaomi Mi 11 Lite NE", brand: "Xiaomi", status: "active", maintainer: "Aistop",
            contributors: ["ETCHDEV, Nikroks, Aistop"],
            testers: ["Aistop"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Crashes on large Transfers", state: "partial" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", description: "SDXC cards don't work", state: "partial" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "working" },
              { feature: "Charging", state: "working" },
              { feature: "WLAN", state: "working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "working" },
              { feature: "Bluetooth", state: "working" },
              { feature: "GPS", state: "working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "working" },
              ],
            },
        ],
      },
      {
        name: "Snapdragon 730/730G/732G Devices",
        devices: [
          { name: "Lenovo Tab P11 Pro", brand: "Lenovo", status: "inactive", maintainer: "None",
          codename: "j706f",
            contributors: ["hyusang"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "unknown" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              //{ feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
          { name: "Xiaomi Poco X3 NFC", brand: "Xiaomi", status: "inactive", maintainer: "remtrik",
            contributors: ["Nikroks, remtrik"],
            testers: ["remtrik"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "working" },
              { feature: "Battery", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
          { name: "Xiaomi Redmi Note 10 Pro", brand: "Xiaomi", status: "inactive", maintainer: "None",
           codename: "sweet",
            contributors: ["remtrik, hiprivsid"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
         },
          { name: "Xiaomi Redmi Note 12 Pro 4G", brand: "Xiaomi", status: "inactive", maintainer: "None",
            codename: "sweet_k6a",
            contributors: ["Robotix"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "unknown" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
          },
        ],
      },
       {
        name: "Snapdragon 720G Devices",
        devices: [
          { 
          name: "Xiaomi Redmi Note 9 Pro / 9 Pro India / 9 Pro Max India / 10 Lite / 9S / Poco M2 Pro", brand: "Xiaomi", status: "active", maintainer: "N1kroks",
            contributors: ["N1kroks"],
            testers: ["arminask"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", state: "working" },
              { feature: "Light Sensor", state: "working" },
              { feature: "Accelerometer Sensor", state: "working" },
              { feature: "Compass Sensor", state: "working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              //{ feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              //{ feature: "Temperature Sensor", state: "working" },
              { feature: "Battery", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "working" },
              { feature: "Bluetooth", state: "working" },
              { feature: "GPS", state: "working" },
              { feature: "Speakers", state: "working" },
              { feature: "3.5mm Audio Jack", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
          { 
          name: "Samsung Galaxy A52 4G", brand: "Samsung", status: "active", maintainer: "arminask",
          codename: "a52q",
            contributors: ["N1krocks"],
            testers: ["arminask"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              //{ feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              //{ feature: "Temperature Sensor", state: "working" },
              { feature: "Battery", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "working" },
              { feature: "WLAN", state: "working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "3.5mm Audio Jack", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "working" },
              ],
           },
        ],
      },
       {
        name: "Snapdragon 695 Devices",
        devices: [
          { name: "Redmi Note 11 Pro 5G", brand: "Xiaomi", status: "active", maintainer: "index986",
            contributors: ["Robotix, index986"],
            testers: ["index986"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "No unplug protection", state: "partial" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "unknown" },
              { feature: "Linux Boot", state: "unknown" },
            ],
          },
        ],
      },
       {
        name: "Snapdragon 680/685 Devices",
        devices: [
          { 
            name: "Xiaomi Redmi 10C", brand: "Xiaomi", status: "inactive", maintainer: "Statzar",
            contributors: ["Statzar"],
            testers: ["Statzar"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", description: "Hot Plug doesn't work", state: "partial" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              //{ feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "3.5mm Audio Jack", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
          { 
            name: "Xiaomi Redmi Note 11", brand: "Xiaomi", status: "inactive", maintainer: "Statzar",
            codename: "spes",
            contributors: ["Statzar"],
            testers: ["Statzar"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              //{ feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "3.5mm Audio Jack", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
          { 
          name: "Xiaomi Redmi Note 12", brand: "Xiaomi", status: "inactive", maintainer: "None",
            contributors: ["Statzar, 6adp"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              //{ feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "3.5mm Audio Jack", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
           },
        ],
      },
       {
        name: "Snapdragon 665 Devices",
        devices: [
          { 
            name: "Xiaomi Mi A3", brand: "Xiaomi", status: "inactive", maintainer: "None",
            codename: "laurel_sprout",
            contributors: ["Kernel357"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "unknown" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
          },
          { 
            name: "Xiaomi Redmi Note 8T", brand: "Xiaomi", status: "inactive", maintainer: "None",
            contributors: ["SwedMlite, Robotix"],
            testers: ["Dima"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "working" },
              { feature: "Mass Storage", description: "Unstable with Device Mode", state: "partial" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "unknown" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              //{ feature: "Compass Sensor", state: "working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              //{ feature: "Hall Sensor", state: "working" },
              //{ feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", description: "4 Cores only", state: "partial" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "3.5mm Audio Jack", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
          { 
            name: "Xiaomi Redmi Note 8", brand: "Xiaomi", status: "inactive", maintainer: "None",
            codename: "ginkgo",
            contributors: ["index986"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
          },
        ],
      },
       {
        name: "Snapdragon 662 Devices",
        devices: [
          { name: "Motorola Moto G30", brand: "Motorola", status: "inactive", maintainer: "None",
            codename: "caprip",
            contributors: ["Vicente Cortes"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", description: "Power button is mapped as SUSPEND", state: "partial" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage",  state: "unknown" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Gyroscope Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              //{ feature: "Hall Sensor", state: "working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", description: "4 Cores only", state: "partial" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "3.5mm Audio Jack", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
          { 
            name: "Motorola Moto G9 Power", brand: "Motorola", status: "inactive", maintainer: "None",
            codename: "cebu",
            contributors: ["remtrik, hiprivsid"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", description: "Needs Device Mode", state: "not-working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
           },
          { 
            name: "Samsung Galaxy Tab A7 LTE (T507)", brand: "Samsung", status: "inactive", maintainer: "V6lhost",
            contributors: ["V6lhost"],
            testers: ["V6lhost"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", description: "Only Volume Buttons", state: "partial" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "working" },
              { feature: "Mass Storage", description: "Reboots", state: "not-working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "unknown" },
              { feature: "Linux Boot", state: "not-working" },
            ],
           },
          { 
            name: "Xiaomi Redmi 9T", brand: "Xiaomi", status: "inactive", maintainer: "None",
            codename: "lime",
            contributors: ["Robotix"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "not-working" },
              { feature: "SD Card", description: "Tries to Init non-existent eMMC instead?", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "unknown" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Gyroscope Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "working" },
              { feature: "USB Device Mode", state: "unknown" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", description: "4 Cores only", state: "partial" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "3.5mm Audio Jack", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
          { 
            name: "Xiaomi Poco M3", brand: "Xiaomi", status: "active", maintainer: "deuspato",
            contributors: ["Robotix, 9.8.7.6, NUC, Aistop, All_One1, Heyylucazzz"],
            testers: ["deuspato"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Gyroscope Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "not-working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", description: "4 Cores only", state: "partial" },
              { feature: "Touchscreen", state: "not-working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "3.5mm Audio Jack", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "not-working" },
              ],
            },
        ],
      },
      {
        name: "Snapdragon 660/636/630 Devices",
        devices: [
          { name: "Nokia 7.2", brand: "Nokia", status: "inactive", maintainer: "creepreee",
            contributors: ["Robotix, creepreee, Aistop"],
            testers: ["creepreee"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "unknown" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "SD Card", state: "unknown" },
              { feature: "Windows Boot", state: "unknown" },
              { feature: "Linux Boot", state: "unknown" },
            ],
           },
          { 
          name: "Xiaomi Mi Max 3", brand: "Xiaomi", status: "inactive", maintainer: "Aistop",
            contributors: ["Robotix", "Aistop"],
            testers: ["Aistop"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", state: "working" },
              { feature: "Mass Storage", state: "working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", state: "working" },
              { feature: "Linux Boot", state: "working" },
            ],
            windowsFeatures: [
              { feature: "Internal Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Side Buttons", state: "not-working" },
              { feature: "Proximity Sensor", state: "not-working" },
              { feature: "Light Sensor", state: "not-working" },
              { feature: "Accelerometer Sensor", state: "not-working" },
              { feature: "Compass Sensor", state: "not-working" },
              { feature: "Fingerprint Sensor", state: "not-working" },
              { feature: "Hall Sensor", state: "not-working" },
              //{ feature: "NFC Sensor", state: "not-working" },
              { feature: "Temperature Sensor", state: "not-working" },
              { feature: "Battery", state: "not-working" },
              { feature: "USB Host Mode", description: "Depends What Mode it was in UEFI, unstable", state: "partial" },
              { feature: "USB Device Mode", description: "Depends What Mode it was in UEFI", state: "working" },
              { feature: "USB Power Delivery", description: "Depends What Mode it was in UEFI", state: "working" },
              { feature: "Charging", state: "not-working" },
              { feature: "WLAN", state: "not-working" },
              { feature: "CPU", state: "working" },
              { feature: "Touchscreen", description: "Only Goodix GT9xx is supported", state: "working" },
              { feature: "Bluetooth", state: "not-working" },
              { feature: "GPS", state: "not-working" },
              { feature: "Speakers", state: "not-working" },
              { feature: "3.5mm Audio Jack", state: "not-working" },
              { feature: "Microphone", state: "not-working" },
              { feature: "GPU", state: "not-working" },
              { feature: "Camera", state: "not-working" },
              { feature: "Mobile Data", state: "not-working" },
              { feature: "Display", state: "working" },
              { feature: "Vibration", state: "working" },
              ],
            },
            //------------------------------------------------------------------------
          { 
          name: "Xiaomi Redmi Note 7", brand: "Xiaomi", status: "inactive", maintainer: "index986",
            contributors: ["index986"],
            testers: ["index986"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "working" },
              { feature: "USB Power Delivery", description: "Unstable if fastboot boot used" , state: "partial" },
              { feature: "Mass Storage", state: "working" },
              { feature: "SD Card", state: "working" },
              { feature: "Windows Boot", description: "Windows PE only", state: "partial" },
              { feature: "Linux Boot", state: "unknown" },
            ],
          },
        ],
      },
    ],
  },
  {
    name: "Exynos",
    icon: "⚡",
    chipsets: [
      {
        name: "Exynos 2400",
        devices: [
          { 
          name: "Galaxy S24", brand: "Samsung", status: "inactive",
          maintainer: "Robotix",
          codename: "e1s",
          contributors: ["Robotix, halal-beef"],
            testers: ["Robotix"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "not-working" },
              { feature: "Side Buttons", description: "Only Volume Up", state: "partial" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              //{ feature: "Mass Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
              ],
          },
        ],
      },
      {
        name: "Exynos 2200",
        devices: [
          { name: "Galaxy S22", brand: "Samsung", status: "working" },
          { name: "Galaxy Tab S8", brand: "Samsung", status: "unknown" },
        ],
      },
      {
        name: "Exynos 2100",
        devices: [
          { name: "Galaxy S21", brand: "Samsung", status: "partial" },
          { name: "Galaxy Note 20", brand: "Samsung", status: "not-working" },
        ],
      },
      {
        name: "Exynos 9830",
        devices: [
          { name: "Samsung Galaxy S20 5G", brand: "Samsung", status: "active" ,
          maintainer: "halal-beef",
          codename: "x1s",
          contributors: ["halal-beef"],
            testers: ["halal-beef"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "not-working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              //{ feature: "Mass Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
              ],
          },
          { name: "Samsung Galaxy Note 20 5G", brand: "Samsung", status: "unknown" ,
          maintainer: "None",
          codename: "c1s",
          contributors: ["Robotix, BotchedRPR"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "not-working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              //{ feature: "Mass Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
              ],
          },
          { name: "Samsung S20 FE", brand: "Samsung", status: "unknown" ,
          maintainer: "None",
          codename: "c1s",
          contributors: ["Robotix, BotchedRPR"],
            testers: ["None"],
            uefiFeatures: [
              { feature: "Display", state: "working" },
              { feature: "Internal Storage", state: "not-working" },
              { feature: "Side Buttons", state: "working" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              //{ feature: "Mass Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
              ],
          },
        ],
      },
      {
        name: "Exynos 9820",
        devices: [
          { name: "Galaxy S10", brand: "Samsung", status: "unknown" },
          { name: "Galaxy Note 10", brand: "Samsung", status: "not-working" },
        ],
      },
    ],
  },
  {
    name: "Tegra",
    icon: "💎",
    chipsets: [
      {
        name: "Tegra X1",
        devices: [
          { name: "Nintendo Switch", brand: "Nintendo", status: "inactive", maintainer: "Robotix",
            codename: "HAC-001",
            contributors: ["Robotix"],
            testers: ["Robotix"],
            uefiFeatures: [
              { feature: "Display", description: "Wrong Orientation" ,state: "partial" },
              { feature: "Internal Storage", state: "not-working" },
              { feature: "Joy Cons", state: "not-working" },
              { feature: "Side Buttons", description: "No Power Button", state: "partial" },
              { feature: "USB Host Mode", state: "not-working" },
              { feature: "USB Device Mode", state: "not-working" },
              { feature: "USB Power Delivery", state: "not-working" },
              //{ feature: "Mass Storage", state: "working" },
              { feature: "SD Card", state: "not-working" },
              { feature: "Windows Boot", state: "not-working" },
              { feature: "Linux Boot", state: "not-working" },
            ],
          },
        ],
      },
    ],
  },
];

const statusConfig = {
  working: { symbol: "✅", label: "Working", color: "bg-green-500/20 text-green-400 border-green-500/30" },
  partial: { symbol: "⚠️", label: "Partial", color: "bg-yellow-500/20 text-yellow-400 border-yellow-500/30" },
  "not-working": { symbol: "❌", label: "Not Working", color: "bg-red-500/20 text-red-400 border-red-500/30" },
  unknown: { symbol: "❔", label: "Unknown", color: "bg-gray-500/20 text-gray-400 border-gray-500/30" },
  active: { symbol: "🟢", label: "Active", color: "bg-blue-500/20 text-blue-400 border-blue-500/30"},
  inactive: { symbol: "🔴", label: "Inactive", color: "bg-neutral-500/20 text-neutral-400 border-neutral-500/30"},
};

const featureStatusConfig = {
  working: { symbol: "✅", color: "text-green-400" },
  "not-working": { symbol: "❌", color: "text-red-400" },
  partial: { symbol: "⚠️", color: "text-yellow-400" },
  unknown: { symbol: "❔", color: "text-gray-400" },
};

export default function Devices() {
  const [searchTerm, setSearchTerm] = useState("");
  const [statusFilter, setStatusFilter] = useState<string>("all");
  const [brandFilter, setBrandFilter] = useState<string>("all");
  const [openManufacturers, setOpenManufacturers] = useState<Set<string>>(new Set());
  const [openChipsets, setOpenChipsets] = useState<Set<string>>(new Set());
  const [openDevices, setOpenDevices] = useState<Set<string>>(new Set());

  const filteredManufacturers = useMemo(() => {
    return manufacturers
      .map(manufacturer => ({
        ...manufacturer,
        chipsets: manufacturer.chipsets
          .map(chipset => ({
            ...chipset,
            devices: chipset.devices.filter(device => {
              const matchesSearch = device.name.toLowerCase().includes(searchTerm.toLowerCase()) ||
                                  device.brand.toLowerCase().includes(searchTerm.toLowerCase()) ||
                                  (device.codename && device.codename.toLowerCase().includes(searchTerm.toLowerCase())) ||
                                  chipset.name.toLowerCase().includes(searchTerm.toLowerCase()) ||
                                  manufacturer.name.toLowerCase().includes(searchTerm.toLowerCase());
              
              const matchesStatus = statusFilter === "all" || device.status === statusFilter;
              const matchesBrand = brandFilter === "all" || device.brand.toLowerCase() === brandFilter.toLowerCase();
              
              return matchesSearch && matchesStatus && matchesBrand;
            })
          }))
          .filter(chipset => chipset.devices.length > 0)
      }))
      .filter(manufacturer => manufacturer.chipsets.length > 0);
  }, [searchTerm, statusFilter, brandFilter]);

  // Auto-expand fields when searching
  React.useEffect(() => {
    if (searchTerm.trim()) {
      const newOpenManufacturers = new Set<string>();
      const newOpenChipsets = new Set<string>();
      
      filteredManufacturers.forEach(manufacturer => {
        newOpenManufacturers.add(manufacturer.name);
        manufacturer.chipsets.forEach(chipset => {
          newOpenChipsets.add(chipset.name);
        });
      });
      
      setOpenManufacturers(newOpenManufacturers);
      setOpenChipsets(newOpenChipsets);
    }
  }, [searchTerm]); // Removed filteredManufacturers to prevent infinite loop

  const toggleManufacturer = (manufacturerName: string) => {
    const newOpenManufacturers = new Set(openManufacturers);
    if (newOpenManufacturers.has(manufacturerName)) {
      newOpenManufacturers.delete(manufacturerName);
    } else {
      newOpenManufacturers.add(manufacturerName);
    }
    setOpenManufacturers(newOpenManufacturers);
  };

  const toggleChipset = (chipsetName: string) => {
    const newOpenChipsets = new Set(openChipsets);
    if (newOpenChipsets.has(chipsetName)) {
      newOpenChipsets.delete(chipsetName);
    } else {
      newOpenChipsets.add(chipsetName);
    }
    setOpenChipsets(newOpenChipsets);
  };

  const toggleDevice = (deviceKey: string) => {
    const newOpenDevices = new Set(openDevices);
    if (newOpenDevices.has(deviceKey)) {
      newOpenDevices.delete(deviceKey);
    } else {
      newOpenDevices.add(deviceKey);
    }
    setOpenDevices(newOpenDevices);
  };

  const allBrands = Array.from(new Set(manufacturers.flatMap(m => m.chipsets.flatMap(c => c.devices.map(d => d.brand)))));

  const DeviceDetailCard = ({ device }: { device: Device }) => {
    const deviceKey = `${device.name}-${device.brand}`;
    const isOpen = openDevices.has(deviceKey);

    return (
      <Card className="bg-background/40 border-border/40">
        <Collapsible 
          open={isOpen}
          onOpenChange={() => toggleDevice(deviceKey)}
        >
          <CollapsibleTrigger asChild>
            <CardHeader className="cursor-pointer hover:bg-muted/20 transition-colors p-4">
              <div className="flex items-center justify-between">
                <div className="flex items-center gap-3">
                  <Smartphone className="h-4 w-4 text-primary" />
                  <div>
                    <div className="font-medium text-left">{device.name}</div>
                    <div className="text-sm text-muted-foreground text-left">{device.brand}</div>
                  </div>
                </div>
                <div className="flex items-center gap-2">
                  <Badge 
                    variant="secondary" 
                    className={`${statusConfig[device.status].color} border`}
                  >
                    <span className="mr-1">{statusConfig[device.status].symbol}</span>
                    {statusConfig[device.status].label}
                  </Badge>
                  {isOpen ? <ChevronDown className="h-4 w-4" /> : <ChevronRight className="h-4 w-4" />}
                </div>
              </div>
            </CardHeader>
          </CollapsibleTrigger>

          <CollapsibleContent>
            <CardContent className="pt-0 px-4 pb-4 space-y-6">
              {/* Device Info */}
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div className="space-y-3">
                  <div className="flex items-center gap-2">
                    <span className="text-sm font-medium">State:</span>
                    {['active', 'inactive'].includes(device.status) && (
  <Badge
    variant="secondary"
    className={
      device.status === 'active'
        ? 'bg-blue-500/20 text-blue-400 border-blue-500/30'
        : 'bg-neutral-500/20 text-neutral-400 border-neutral-500/30'
    }
  >
    {device.status === 'active' ? 'Active' : 'Inactive'}
  </Badge>
)}
                  </div>
                  
                  {device.codename && (
                    <div className="flex items-center gap-2">
                      <span className="text-sm font-medium">Codename:</span>
                      <span className="text-sm text-muted-foreground font-mono">{device.codename}</span>
                    </div>
                  )}
                </div>

                <div className="space-y-3">
                  {device.maintainer && (
                    <div className="flex items-center gap-2">
                      <User className="h-4 w-4 text-muted-foreground" />
                      <span className="text-sm font-medium">Maintainer:</span>
                      <span className="text-sm text-muted-foreground">{device.maintainer}</span>
                    </div>
                  )}
                  
                  {device.contributors && device.contributors.length > 0 && (
                    <div className="flex items-start gap-2">
                      <Users className="h-4 w-4 text-muted-foreground mt-0.5" />
                      <div>
                        <span className="text-sm font-medium">Contributors:</span>
                        <div className="text-sm text-muted-foreground">{device.contributors.join(", ")}</div>
                      </div>
                    </div>
                  )}
                  
                  {device.testers && device.testers.length > 0 && (
                    <div className="flex items-center gap-2">
                      <TestTube className="h-4 w-4 text-muted-foreground" />
                      <span className="text-sm font-medium">Testers:</span>
                      <span className="text-sm text-muted-foreground">{device.testers.join(", ")}</span>
                    </div>
                  )}
                </div>
              </div>

              {/* UEFI Status */}
              {device.uefiFeatures && device.uefiFeatures.length > 0 && (
                <div>
                  <h4 className="text-lg font-semibold mb-3 flex items-center gap-2">
                    <Cpu className="h-5 w-5 text-primary" />
                    UEFI Status
                  </h4>
                  <div className="rounded-lg border border-border/50 overflow-hidden">
                    <Table>
                      <TableHeader>
                        <TableRow className="bg-muted/30">
                          <TableHead className="font-semibold">Feature</TableHead>
                          <TableHead className="font-semibold">Description</TableHead>
                          <TableHead className="font-semibold text-center">State</TableHead>
                        </TableRow>
                      </TableHeader>
                      <TableBody>
                        {device.uefiFeatures.map((feature, index) => (
                          <TableRow key={index} className="hover:bg-muted/20">
                            <TableCell className="font-medium">{feature.feature}</TableCell>
                            <TableCell className="text-muted-foreground">
                              {feature.description || ""}
                            </TableCell>
                            <TableCell className="text-center">
                              <span className={`text-lg ${featureStatusConfig[feature.state].color}`}>
                                {featureStatusConfig[feature.state].symbol}
                              </span>
                            </TableCell>
                          </TableRow>
                        ))}
                      </TableBody>
                    </Table>
                  </div>
                </div>
              )}

              {/* Windows Status */}
              {device.windowsFeatures && device.windowsFeatures.length > 0 && (
                <div>
                  <h4 className="text-lg font-semibold mb-3 flex items-center gap-2">
                    <Smartphone className="h-5 w-5 text-primary" />
                    OS Status – Windows
                    <Badge variant="secondary" className="bg-yellow-500/20 text-yellow-400 border-yellow-500/30 text-xs">
                      Needs Windows Drivers
                    </Badge>
                  </h4>
                  <div className="rounded-lg border border-border/50 overflow-hidden">
                    <Table>
                      <TableHeader>
                        <TableRow className="bg-muted/30">
                          <TableHead className="font-semibold">Feature</TableHead>
                          <TableHead className="font-semibold">Description</TableHead>
                          <TableHead className="font-semibold text-center">State</TableHead>
                        </TableRow>
                      </TableHeader>
                      <TableBody>
                        {device.windowsFeatures.map((feature, index) => (
                          <TableRow key={index} className="hover:bg-muted/20">
                            <TableCell className="font-medium">{feature.feature}</TableCell>
                            <TableCell className="text-muted-foreground">
                              {feature.description || ""}
                            </TableCell>
                            <TableCell className="text-center">
                              <span className={`text-lg ${featureStatusConfig[feature.state].color}`}>
                                {featureStatusConfig[feature.state].symbol}
                              </span>
                            </TableCell>
                          </TableRow>
                        ))}
                      </TableBody>
                    </Table>
                  </div>
                </div>
              )}
            </CardContent>
          </CollapsibleContent>
        </Collapsible>
      </Card>
    );
  };

  return (
    <div className="min-h-screen bg-background p-4 md:p-6">
      <div className="max-w-6xl mx-auto space-y-6">
        {/* Header */}
        <div className="text-center space-y-2">
          <h1 className="text-3xl md:text-4xl font-bold bg-gradient-to-r from-primary to-primary/60 bg-clip-text text-transparent">
            UEFI Device Compatibility
          </h1>
          <p className="text-muted-foreground text-lg">
            Check UEFI support status for Android smartphones by chipset
          </p>
        </div>

        {/* Search and Filters */}
        <Card className="backdrop-blur-sm bg-card/50 border-border/50">
          <CardContent className="p-6">
            <div className="flex flex-col md:flex-row gap-4">
              <div className="relative flex-1">
                <Search className="absolute left-3 top-1/2 transform -translate-y-1/2 text-muted-foreground h-4 w-4" />
                <Input
                  placeholder="Search devices, brands, or chipsets..."
                  value={searchTerm}
                  onChange={(e) => setSearchTerm(e.target.value)}
                  className="pl-10 bg-background/50"
                />
              </div>
              
              <div className="flex gap-3">
                <Select value={statusFilter} onValueChange={setStatusFilter}>
                  <SelectTrigger className="w-[140px] bg-background/50">
                    <SelectValue placeholder="Status" />
                  </SelectTrigger>
                  <SelectContent>
                    <SelectItem value="all">All Status</SelectItem>
                    <SelectItem value="working">Working</SelectItem>
                    <SelectItem value="problematic">Problematic</SelectItem>
                    <SelectItem value="not-working">Not Working</SelectItem>
                {/*To do
                    <SelectItem value="not-working">Active</SelectItem>
                    */}
                    <SelectItem value="unknown">Unknown</SelectItem>
                  </SelectContent>
                </Select>

                <Select value={brandFilter} onValueChange={setBrandFilter}>
                  <SelectTrigger className="w-[120px] bg-background/50">
                    <SelectValue placeholder="Brand" />
                  </SelectTrigger>
                  <SelectContent>
                    <SelectItem value="all">All Brands</SelectItem>
                    {allBrands.map(brand => (
                      <SelectItem key={brand} value={brand.toLowerCase()}>{brand}</SelectItem>
                    ))}
                  </SelectContent>
                </Select>
              </div>
            </div>
          </CardContent>
        </Card>

        {/* Legend | the info on the start actually*/}
        <Card className="backdrop-blur-sm bg-card/50 border-border/50">
          <CardHeader>
            <CardTitle className="text-lg flex items-center gap-2">
              <Filter className="h-5 w-5" />
              Status Legend
            </CardTitle>
          </CardHeader>
          <CardContent>
            <div className="rounded-lg border border-border/50 overflow-hidden">
              <Table>
                <TableHeader>
                  <TableRow className="bg-muted/30">
                    <TableHead className="font-semibold text-center w-20">Symbol</TableHead>
                    <TableHead className="font-semibold">Meaning</TableHead>
                  </TableRow>
                </TableHeader>
                <TableBody>
                  {Object.entries(statusConfig).map(([key, config]) => (
                    <TableRow key={key} className="hover:bg-muted/20">
                      <TableCell className="text-center text-lg">
                        {config.symbol}
                      </TableCell>
                      <TableCell className="font-medium">
                        {config.label}
                      </TableCell>
                    </TableRow>
                  ))}
                </TableBody>
              </Table>
            </div>
          </CardContent>
        </Card>

        {/* Manufacturer Sections */}
        <div className="space-y-6">
          {filteredManufacturers.map((manufacturer) => (
            <Card key={manufacturer.name} className="backdrop-blur-sm bg-card/50 border-border/50 overflow-hidden">
              <Collapsible 
                open={openManufacturers.has(manufacturer.name)}
                onOpenChange={() => toggleManufacturer(manufacturer.name)}
              >
                <CollapsibleTrigger asChild>
                  <CardHeader className="cursor-pointer hover:bg-muted/50 transition-colors p-6">
                    <div className="flex items-center justify-between">
                      <div className="flex items-center gap-4">
                        <div className="text-3xl">{manufacturer.icon}</div>
                        <div>
                          <CardTitle className="text-2xl">{manufacturer.name}</CardTitle>
                          <p className="text-sm text-muted-foreground">
                            {manufacturer.chipsets.length} chipset{manufacturer.chipsets.length !== 1 ? 's' : ''} • {manufacturer.chipsets.reduce((acc, chipset) => acc + chipset.devices.length, 0)} devices
                          </p>
                        </div>
                      </div>
                      <Cpu className="h-6 w-6 text-muted-foreground" />
                    </div>
                  </CardHeader>
                </CollapsibleTrigger>

                <CollapsibleContent>
                  <CardContent className="pt-0 px-6 pb-6">
                    <div className="space-y-4">
                      {manufacturer.chipsets.map((chipset) => (
                        <Card key={chipset.name} className="bg-background/30 border-border/30">
                          <Collapsible 
                            open={openChipsets.has(chipset.name)}
                            onOpenChange={() => toggleChipset(chipset.name)}
                          >
                            <CollapsibleTrigger asChild>
                              <CardHeader className="cursor-pointer hover:bg-muted/30 transition-colors p-4">
                                <div className="flex items-center justify-between">
                                  <div className="flex items-center gap-3">
                                    <Cpu className="h-5 w-5 text-primary" />
                                    <div>
                                      <CardTitle className="text-lg">{chipset.name}</CardTitle>
                                      <p className="text-sm text-muted-foreground">
                                        {chipset.devices.length} device{chipset.devices.length !== 1 ? 's' : ''}
                                      </p>
                                    </div>
                                  </div>
                                  <div className="flex gap-2">
                                    {Array.from(new Set(chipset.devices.map(d => d.status))).map(status => (
                                      <div key={status} className="text-lg">
                                        {statusConfig[status].symbol}
                                      </div>
                                    ))}
                                  </div>
                                </div>
                              </CardHeader>
                            </CollapsibleTrigger>

                            <CollapsibleContent>
                              <CardContent className="pt-0 px-4 pb-4">
                                <div className="space-y-3">
                                  {chipset.devices.map((device) => (
                                    <DeviceDetailCard
                                      key={`${device.name}-${device.brand}`}
                                      device={device}
                                    />
                                  ))}
                                </div>
                              </CardContent>
                            </CollapsibleContent>
                          </Collapsible>
                        </Card>
                      ))}
                    </div>
                  </CardContent>
                </CollapsibleContent>
              </Collapsible>
            </Card>
          ))}
        </div>

        {filteredManufacturers.length === 0 && (
          <Card className="backdrop-blur-sm bg-card/50 border-border/50">
            <CardContent className="text-center py-12">
              <Smartphone className="h-12 w-12 text-muted-foreground mx-auto mb-4" />
              <h3 className="text-lg font-semibold mb-2">No devices found</h3>
              <p className="text-muted-foreground">
                Try adjusting your search terms or filters
              </p>
            </CardContent>
          </Card>
        )}
      </div>
    </div>
  );
}

