# Getting Started Guide for Espressif AWS IoT ExpressLink Evaluation Kit

# Table of Contents

1. [Document Information](#1-document-information)
2. [Overview](#2-overview)
3. [Hardware Description](#3-hardware-description)
4. [Setup your AWS account and Permissions for IoT development](#4-setup-your-aws-account-and-permissions-for-iot-development)
5. [Registering an Espressif AWS IoT ExpressLink to your development account](#5-registering-an-espressif-aws-iot-expresslink-to-your-development-account)
6. [Connecting and Interacting with AWS cloud](#6-connecting-and-interacting-with-aws-cloud)
7. [Using Espressif AWS IoT ExpressLink with the Arduino Sketch](#7-using-espressif-aws-iot-expresslink-with-the-arduino-sketch)
8. [Upgrading the Espressif AWS IoT ExpressLink firmware](#8-upgrading-the-espressif-aws-iot-expresslink-firmware)
9. [Troubleshooting](#9-troubleshooting)

# 1. Document Information

## 1.1 Revision History (Version, Date, Description of change)

1.0 29-November-2021 Final Draft

# 2. Overview

![Espressif AWS IoT ExpressLink Eval Board](_static/ESP-AWS-IoT-ExpressLink.png "Espressif AWS IoT ExpressLink Eval Kit")

The Espressif AWS IoT ExpressLink module is a connectivity module that connects via a serial interface (UART) and uses an abstracted Application Programming Interface (API) to connect any host application to AWS IoT Core and its services.
In so doing, the Espressif AWS IoT ExpressLink module offloads complex and undifferentiated workload such as authentication, device management, connectivity, and messaging from the application (host) processor.
It enables a scalable migration for millions of embedded applications to cloud-connected applications and offers fast time to market.  
More information on AWS IoT ExpressLink can be found [here](https://aws.amazon.com/iot-expresslink/).  
The developer documentation can be found [here](https://docs.aws.amazon.com/iot-expresslink).  
Examples for AWS IoT Expresslink can be found [here](https://github.com/aws/iot-expresslink).  

# 3. Hardware Description

The Espressif AWS IoT ExpressLink board has an Arduino shield form factor, so it can be plugged directly on top of the standard Arduino.
It can also be used with a Raspberry Pi or any other host.  
It has the following connection pins that need to be made, for full functionality of the board:  

| Espressif AWS IoT ExpressLink Pin | ESP32-C3 GPIO Pin | ESP32-C3-MINI-1 Module Pin |
|-----------------------------------|-------------------|----------------------------|
| TX                                | IO19              | 27                         |
| RX                                | IO18              | 26                         |
| EVENT                             | IO10              | 16                         |
| WAKE                              | IO3               | 6                          |
| RESET                             | EN                | 8                          |

## 3.1 DataSheet

The Espressif AWS IoT ExpressLink board is based on the ESP32-C3-Mini-1.  
The Datasheet can be found [here](https://www.espressif.com/sites/default/files/documentation/esp32-c3-mini-1_datasheet_en.pdf).

## 3.2 Standard Kit Contents

- One unit of the Espressif AWS IoT ExpressLink board.

## 3.3 User Provided items

- Arduino or Raspberry Pi  
- Any development host

You may also need the following items:  
- MicroUSB cable such as [this](https://www.amazon.com/AmazonBasics-Male-Micro-Cable-Black/dp/B0711PVX6Z/).  
- Jumper cables such as [these](https://www.amazon.com/Elegoo-EL-CP-004-Multicolored-Breadboard-arduino/dp/B01EV70C78/).  
- USB TTL converter such as [this one](https://www.amazon.com/Adapter-Serial-Converter-Development-Projects/dp/B075N82CDL/).

## 3.4 3rd Party purchasable items

[Arduino purchase link](https://store-usa.arduino.cc/collections/boards/products/arduino-uno-rev3)  
[Raspberry Pi purchase link](https://www.raspberrypi.com/products/)  

## 3.5 Additional Hardware References

Refer to the [ESP32-C3 Hardware Reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/hw-reference/esp32c3/user-guide-devkitm-1.html#hardware-reference), for more information on the hardware.

## 3.6 Set up your hardware

## 3.6.1 With Arduino

1. For using the Espressif AWS IoT ExpressLink Board with an Arduino board that can be used with shields, there is no additional setup required.
2. You can directly plug the Espressif AWS IoT ExpressLink Board onto the Arduino board's headers.
3. Once this is done, ensure that the Espressif AWS IoT ExpressLink Board's switch is in OFF state.
4. You may now connect the Arduino to your computer, and that will automatically power the Board.
5. You can use the Arduino sketch as shown in [Section 7](#7-using-espressif-aws-iot-expresslink-with-the-arduino-sketch) to quickly get started.  

The pins of the Espressif AWS IoT ExpressLink Board are mapped to the Arduino as follows:  

| Espressif AWS IoT ExpressLink Pin | Arduino Pin |
|-----------------------------------|-------------|
| RESET                             | 4           |
| WAKE                              | 3           |
| EVENT                             | 2           |
| RX                                | 1           |
| TX                                | 0           |
| IOREF                             | IOREF       |
| GND                               | GND         |

## 3.6.2 With Raspberry Pi

1. To connect an Espressif AWS IoT ExpressLink Board to the Raspberry Pi, you need to connect the TX, RX, EVENT, WAKE and RESET male pins of the J13 connector on the Espressif AWS IoT ExpressLink Board to the following GPIO pins of the Raspberry Pi (using female-to-female jumpers):

| Espressif AWS IoT ExpressLink Pin | Raspberry Pi GPIO |
|-----------------------------------|-------------------|
| RESET                             | GPIO 4            |
| WAKE                              | GPIO 27           |
| EVENT                             | GPIO 22           |
| RX                                | GPIO 15           |
| TX                                | GPIO 14           |
| IOREF                             | 3V3 Power         |
| GND                               | GND               |

2. You can now access the Espressif AWS IoT ExpressLink on your Raspberry Pi, by using a desktop terminal application of your choice and using /dev/ttyS0 with the settings as shown in the table in Section 3.7

## 3.6.3 With any development host

The Espressif AWS IoT ExpressLink board can be used with any development host, over a USB serial interface (using a USB-to-TTL converter) and using the simple AT commands for controlling Espressif AWS IoT ExpressLink.

| Espressif AWS IoT ExpressLink Pin | USB-to-TTL converter |
|-----------------------------------|----------------------|
| RX                                | RX                   |
| TX                                | TX                   |
| GND                               | GND                  |

> Please note that, in this case, additional functionality like WAKE and EVENT cannot be utilised, but this can serve as a simple way for a quick evaluation and understanding behaviour of commands.

## 3.7 Set up host machine

To establish a serial connection between your host machine and the Espressif AWS IoT ExpressLink, open a desktop terminal application for your host machine (e.g., TeraTerm for Windows, CoolTerm for Mac) and select the port corresponding to the evaluation kit. Configure the desktop terminal application as follows:  

| Config Option | Value               |
|---------------|---------------------|
| Baudrate      | 115200              |
| Bits          | 8                   |
| Parity        | None                |
| Stopbit       | 1                   |
| Flow control  | None                |
| Local Echo    | Yes                 |
| End of Line   | Line Feed           |

For a quick check, in the terminal window type: **AT** followed by **return**. If you receive the answer **OK** , Congratulations! You have successfully connected the evaluation kit to your host machine.

> Please note that it is **NOT** possible to use the Espressif AWS IoT ExpressLink board using **ONLY** the connections shown in step 5a, i.e. it is **NOT** possible to give the AT commands using the console where you can see the Espressif AWS IoT ExpressLink bootup log. You will need to do additional steps as shown in 3.6.2 and 3.6.3 in case of Raspberry Pi and hosts other than Arduino. 

Keep the terminal open, as it is needed for subsequent steps.

# 4. Setup your AWS account and Permissions for IoT development

Refer to the instructions at [Set up your AWS Account](https://docs.aws.amazon.com/iot/latest/developerguide/setting-up.html). Follow the steps outlined in these sections to create your account and a user and get started:

- Sign up for an AWS account and
- Create a user and grant permissions.
- Open the AWS IoT console

Pay special attention to the Notes.

# 5. Registering an Espressif AWS IoT ExpressLink to your development account

To create an IoT _Thing_ and add it to your account we will need to retrieve the Espressif AWS IoT ExpressLink module Thing Name and its corresponding certificate.  
There are two methods to obtain the certificate, as shown in steps 5a and 5b:  

## 5a. Directly connecting the Espressif AWS IoT ExpressLink board to your computer

1. Remove any other connections you have made to the board.
2. Connect the Espressif AWS IoT ExpressLink board to the computer, using the microUSB port on the board, and a microUSB to USB cable.
3. Open the desktop terminal application on the host machine, and select the UART port of the board, and set baud rate to 115200.
4. Press the Reset button on the board, and ensure that you can see the bootup log.
5. The bootup log will clearly display the device certificate and the Thing Name towards the end.
6. Make a note of the Thing Name and copy the certificate from the `-----BEGIN CERTIFICATE-----` line till the `-----END CERTIFICATE-----` line and save it to a file named `ThingName.cert.pem`

## 5b. Using AT Commands to obtain Certificate and Thing Name

1. In the desktop terminal application type the command: **AT+CONF? ThingName**
2. Make a note of the returned string (a sequence of alphanumeric characters) from the terminal.
3. In the desktop terminal application type the command: **AT+CONF? Certificate pem**
4. Copy the returned string (a longer sequence of alphanumeric symbols), save into a text file on your host machine as "ThingName.cert.pem".

## 5c. Setting up on the AWS IoT Console

1. Open the [AWS IoT Console](http://console.aws.amazon.com/iot). Select **Manage** then select **Things**. Choose **Create things** , select **Create single thing** , click **Next**.  
2. On the **Specify thing properties** page, paste the previously copied ThingName (from step 5b) from terminal into the **Thing name** under **Thing properties** on the console. Leave other fields as default, then click **Next**.
3. On the **Configure device certificate page** , select **Use my certificate** , choose **CA is not registered with AWS IoT.**
4. Under **Certificate** , select **Choose file**. Double click on "ThingName.cert.pem" file in step 5a or 5b.
5. Under **Certificate Status** , select **Active**
6. Click **Next** to **Attach policies to certificate**.
7. Under **Secure** , select **Policies.**
8. Click **Create** to **Create a policy**. Put policy name (e.g. IoTDevPolicy) and click **Advanced mode**.
9. Copy the below section into the console.

`
{ "Version": "2012-10-17", "Statement": [{ "Effect": "Allow", "Action": "*", "Resource": "*" }] }
`

_NOTE – The examples in this document are intended only for dev environments. All devices in your fleet must have credentials with privileges that authorize only intended actions on specific resources. The specific permission policies can vary for your use case. Identify the permission policies that best meet your business and security requirements. For more information, refer to Example policies and Security Best practices._

Click **Save** to complete the Thing creation.

1. In the AWS IoT Console, choose **Settings** , copy your account _Endpoint_ string under _Device data endpoint._
2. In the desktop terminal application type the command: **AT+CONF Endpoint=<_replace-with-your-endpoint-string_>**

## 5.1 Set up for Wi-Fi

The Espressif AWS IoT ExpressLink Board requires access to a local Wi-Fi router in order to connect to the Internet. You can enter the required security credentials in two ways, as shown in steps 5.1.1 and 5.1.2  

**Note** : When the Wi-Fi on the Espressif AWS IoT ExpressLink Board is not configured, the board will try to connect to the following SSID: **ESP-ExpressLink-Demo** and Passphrase: **ExpressLink@12345** by default.  

## 5.1.1 Using CONFMODE  

1. The Espressif AWS IoT ExpressLink board can be easily configured with a mobile device, using Espressif's open source provisioning apps, available on the Google Play Store for Android, and the Apple App Store for iOS and iPadOS.  

[Google Play Store link](https://play.google.com/store/apps/details?id=com.espressif.provble)  
[Apple App Store link](https://apps.apple.com/app/esp-ble-provisioning/id1473590141)

Both the apps are open source, and the source code for the apps is available on GitHub.  

[Android Provisioning App on GitHub](https://github.com/espressif/esp-idf-provisioning-android)  
[iOS/iPadOS Provisioning App on GitHub](https://github.com/espressif/esp-idf-provisioning-ios)

2. In the desktop terminal application, type the command: **AT+CONFMODE**  
You will receive an "OK CONFMODE ENABLED" on the host.  
3. Now, open the Espressif provisioning app on your mobile device that you have installed in the previous step, and click on the "Provision Device" button.
4. Then click on "I don't have a QR code".
5. The app will now search for active BLE devices.
6. When you see your device, click on its name.
7. You will now see a list of the 2.4 Ghz Wi-Fi networks that the Espressif AWS IoT ExpressLink board can see.  
Select the Wi-Fi network that you would like to connect to, and enter the credentials.
8. The credentials will be sent and saved to the device.

## 5.1.3 Using AT Commands  

1. In the desktop terminal application type the command: **AT+CONF SSID=<_replace-with-your-router-ssid_>**
2. In the desktop terminal application type the command: **AT+CONF Passphrase=<_replace-with-your-router-passphrase_>**

**Note** : Your local router's SSID and passphrase are stored securely inside the Espressif AWS IoT ExpressLink module. While the SSID can be retrieved later (i.e., for debugging purposes) any attempt to retrieve the Passphrase will return an error.

## 5.2 Completion

Congratulations! You have completed the registration of the evaluation kit as a Thing in your IoT account. You will not need to repeat these steps the next time you connect, as the Espressif AWS IoT ExpressLink module will remember its configuration and will be ready to connect to your AWS account automatically.

# 6. Connecting and Interacting with AWS cloud

We will use the MQTT client in the AWS IoT console to help us monitor the communication between your evaluation kit and the AWS Cloud.

1. Navigate to the AWS IoT console ([https://console.aws.amazon.com/iot/](https://console.aws.amazon.com/iot/)).
2. To open the MQTT client, in the navigation pane, choose **Test** and then **MQTT Test Client.**
3. In _Subscribe to a topic_, enter #, and then click **Subscribe**.

## 6.1 Connecting

Establish a secure connection by entering the command: **AT+CONNECT**

After a short time, you will receive the message **OK 1 CONNECTED**

Congratulations! You have successfully connected to your AWS cloud account.

## 6.2 Sending data to the AWS cloud

To send "Hello World!" message, enter the command: **AT+SEND data Hello World!**

After a short time, you will receive the message **OK**

You should see the "Hello World!" message appear on the AWS IoT console under the topic "data".

## 6.3 Receiving data and commands from the cloud

To receive messages, first enter the command: **AT+CONF Topic1=MyTopic**

You will receive the response from the module: **OK**

Next, enter the command: **AT+SUBSCRIBE1**

From the AWS IoT console, MQTT client, select **Publish to a topic** , type **MyTopic** in **Topic name** field, keep **"Hello from the AWS IoT console"** message then click "Publish".

On your terminal, enter the command: **AT+GET1**

You will receive the message **OK Hello from the AWS IoT console**

# 7. Using Espressif AWS IoT ExpressLink with the Arduino Sketch

We have provided a basic sketch in this repository (arduino_sample_sketch.ino) to get started quickly, with an Arduino.  
The sketch will do the following:  
- It will reset the board and wait till it is ready.
- It will check if the board is already provisioned (i.e. if it has credentials to connect to a Wi-Fi network already) and it will set the hardcoded EndPoint.
- If it does not have the credentials, it will enter CONFMODE (Refer Section 5.1.1 for steps to use this).
- Once it has been provisioned, it will try to connect to the Wi-Fi network.
- Once the connection has been successfully established, it will send "Hello World" to the "TEST" topic, every 10 seconds.  

Before you flash the provided sketch, change the hardcoded value of the EndPoint in the script the AWS IoT EndPoint that you obtained in Step 5c.

You can flash the sketch onto the Arduino board, using the steps shown [here](https://www.arduino.cc/en/main/howto).

> Note that the sketch is just a starting point, and is not the only way to use the Espressif AWS IoT ExpressLink Board with an Arduino.  

## 7.1 Debugging the Arduino sketch

Arduino's standard RX and TX pins are used to communicate with Espressif AWS IoT ExpressLink board.  
Hence, standard USB interface of Arduino cannot be used for logging and debugging.  
You can use any other UART of Arduino for debugging purpose, as described below:  

1. In order to debug your Arduino sketch, you will need to use a USB-to-TTL converter (purchase link can be found in section 3.3)
2. For logging anything from the sketch, you will need to connect pins of the Arduino (via the Espressif AWS IoT ExpressLink board) to the USB-to-TTL converter as shown in the table below:  

| Espressif AWS IoT ExpressLink Pin | USB-to-TTL converter Pin | Arduino Pin |
|-----------------------------------|--------------------------|-------------|
| RX                                | RX                       | 8           |
| TX                                | TX                       | 9           |
| GND                               | GND                      | GND         |

3. Use a desktop terminal application set at baudrate 115200 and the port of the USB-to-TTL converter to view the output.
The code snippet below shows how to accomplish this in an Arduino sketch.

```cpp
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 9); // RX, TX

void setup()
{
    mySerial.begin(115200);
    while (!mySerial) {
        ;
    }
}

void loop()
{
    mySerial.print("Hello World!");
    delay(2000);
}
``` 

# 8. Upgrading the Espressif AWS IoT ExpressLink firmware

In order to upgrade the Espressif AWS IoT ExpressLink Board to the latest available firmware, you can follow any one of the approaches shown in Section 8.1.1 and 8.1.2

## 8.1 Carrying out an Over-The-Air (OTA) Upgrade

## 8.1.1 Prerequisites

1. Download the latest release for Espressif AWS IoT ExpressLink from the releases section.  
Ensure that it has the following contents:  
a) A metadata file indicating the signature hashing algorithm used (Example: SHA-256), the signature encryption algorithm used (Example: ECDSA)  
b) The signature file, encoded using the base64 encoding format.   
c) The firmware binary.  

2. Create an OTA Update role in your account using the steps outlined [here](https://docs.aws.amazon.com/freertos/latest/userguide/create-service-role.html).

## 8.1.2 Creating a Firmware Update job in AWS IoT

- Open [AWS IoT Console](http://console.aws.amazon.com/iot). Select **Manage** then select **Jobs**. Choose **Create job** , select **Create FreeRTOS OTA Update Job** , click **Next**.
- Provide a job name which is unique within your AWS account. Provide an optional description. Click **Next**.
- From **Devices to update** dropdown choose the thing name with which the Express Link is registered with the account. Select **MQTT** as the protocol to transfer, and unselect **HTTP** if selected.
- Choose **Use my custom signed file** which will display a form to be filled. Use the details from the prerequisites to fill the form.
- In the **signature** field provide the base64 encoded signature for the image. From the **Original hashing algorithm** drop down, select the hashing algorithm provided by the manufacturer. From the **Original encryption algorithm** drop down, select the encryption algorithm provided by the manufacturer. For **Path name of code signing certificate on device**, just enter **NA**.
- Select **Upload a new file** , click on **Choose file** and upload the firmware binary. Select **Create S3 bucket** for the new uploaded image and proceed with creating a new bucket. If needed you can also choose an existing bucket in your account by selecting **Browse S3** option.
- Under **Path Name of file on device** you can enter **NA**.
- Under **File type** drop down select a value (default to 0) to signify it's an express link firmware update as opposed to a host firmware update.
- Choose the OTA update role created above from **role** dropdown under **IAM role** section. Click **Next**.
- Click **Create Job**. On successful creation it should list the job name and state as in progress.

## 8.1.3 Monitoring and Applying new firmware update for ExpressLink

You need to add the signature obtained previously, to the Espressif AWS IoT ExpressLink board so that the firmware can be verified.  
Start by giving the command:  
**AT+CONF OTACertificate=PEM**  
The module will respond with "OK" and go into "Certificate writing" mode.  
You can then paste the contents of the signature.crt file in your terminal.  
You should see "OK COMPLETE" at the end.  

- Express link module polls for firmware update jobs, receives and validates a job, and enters a state waiting for update to be accepted.
- Host application receives an OTA event indicating a new firmware image is available for ExpressLink.
- Host application can query the state of the job using the command **AT+OTA?**
- Host application can accept a new firmware update for by issuing command **AT+OTA ACCEPT**
- ExpressLink should now start downloading the firmware update from cloud. Host can monitor the state of the job using **AT+OTA?**
- On download completion and successful image signature validation, host receives an event to apply the new image.
- Host application can apply the new image by issuing the command **AT+OTA APPLY**
- Express Link now reboots and boots up with the new image. Host receives a **STARTUP** event indicating the new image is booted.
- Host application can connect back to the AWS IoT by issuing **AT+CONNECT**
- ExpressLink should now connect to AWS IoT, complete the self-test and mark the image as valid preventing any further rollback to old image.
- Going back to the AWS IoT console, job status should be marked as completed and succeeded.

**Note** : You need to run **AT+CONNECT** after the OTA update has been applied in order to complete the OTA. Failure to do so during the first bootup of the new firmware, will result in a rollback to the previous firmware.

## 8.2 Carrying out an Over-The-Wire (OTW) Upgrade

To facilitate situations in which OTA upgrades are difficult to setup or completely not possible due to poor connectivity, we have provided the facility to carry out OTW upgrades.  
The file `otw.py` needed for carrying out the OTW upgrade is available in this repository.  

1. Start by connecting the board to your computer, as shown in Section 3.6.3
2. Ensure that you have a functional Python 3.X installation.
3. Install PySerial by typing `pip3 install pyserial==3.5`
4. You can start the OTW process by typing `python3 otw.py (UART serial port) (Firmware binary filename)`
5. You should see percentage completion on the python console
6. The update can take around 2 minutes to complete, and you will see `Uploaded 100.0%b'OK\n' Done...` on the console once it is complete.
7. Reset the board now, to boot into the latest firmware.

# 9. Troubleshooting

1. If you are not able to establish connection over the UART using microUSB port on the Espressif AWS IoT ExpressLink board in step 5a, you will need to download the drivers for it and check the additional information for your operating system in the [Establish Serial Connection](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/get-started/establish-serial-connection.html) guide.

2. For common AT command issues, please refer to AWS IoT ExpressLink FAQ page.