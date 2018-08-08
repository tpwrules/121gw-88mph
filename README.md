## Introduction
Welcome to 88mph, the unofficial firmware for the [EEVBlog 121GW](https://www.eevblog.com/product/121gw/) multimeter. This project aims to make available the full functionality of the meter, while prioritizing understandability, usability, and hackability.

*"If my programming is correct, when this meter reaches 88mph, you're going to measure some serious s#!t!"*<br />
~ Doc Brown (paraphrased)

**DISCLAIMER:** If you intend to use this firmware in situations where an incorrect or misleading reading could put you or others at risk of injury or financial loss, then this is not the firmware for you. Please use the official firmware, or another meter entirely.

## Installation and Usage

Unfortunately, 88mph is not yet functional or stable enough for a binary release. Please check back later!

## Contributing

Unfortunately, 88mph is not yet functional or stable enough to be worthy of contributions. I need to do more work and establish the conventions I want others to follow. Please check back later!

## Project Goals

I think these goals can help guide the project to something useful for everyone. I'm not saying that help will be rejected which does not meet every letter of every goal, but I intend for myself and others to refer to them when making choices. Specific ideas I have in mind to accomplish these goals are also written below.

1. Transparency: The meter's behavior should be easy to understand and explain, and well documented.
    * Consolidate and generalize behavior. The antithesis of the official firmware.
    * Keep code modular, and carefully document interactions between modules.
    * Inform the user of illegal operations and invalid states. Document why these might happen and how to resolve them.
2. Testability: The meter's behavior should be backed up by scientific proofs and experiments that anyone can verify (with appropriate equipment). Choices in measurement algorithms and settings should be made with scientifically sound and testable reasoning.
    * Documented procedures to perform specific tests. Work with forum members to develop effective tests.
    * Research why the existing firmware works the way it does, and how it can be improved.
3. Usability: The meter itself should remain easy to pick up and use. Sensible defaults should be in place, and advanced customization can be left to SD card configuration files or a BLE app.
    * Better use of buttons and setup menu
    * Use the factory calibration data as much as possible, but offer a way to improve it
    * Way to access official firmware from 88mph. Will talk with Dave about this.
4. Hackability: The firmware should be designed with modification in mind. The steps required to change or add behavior should be clear. Additionally, infrastructure must be in place to support the testing and experimentation required for the other goals.
    * Modularity will help here. Allow additions to tables and such so that behavior can be extended.
    * Create Bluetooth console to change every setting and detail so that specific experiments can be set up and documented and don't require reprogramming to fiddle.

The project also has anti-goals, which are goals that the project will specifically avoid.
1. Absolute performance: Prefer an understandable but less accurate solution over an unexplainable but more accurate one. Similarly, prefer a simple and modular but slower solution to one that is faster but total spaghetti code.
    * It is my belief that any solution can be retooled to honor the above goals. This rule is to prevent lazy hacks that will complicate the future.
2. Identical behavior: The behavior documented in the official manual will be used as a guide, but will not necessarily be adhered to. Doing so would limit the freedom to fix usability niggles. The exception is that the buttons and switches must continue to work as documented on the meter body itself.
    * Regarding the exception, let's think about the HOLD button. In 88mph, pressing the HOLD button will still activate the HOLD function. However, the operation of the function may be different, and of course documented. Perhaps the sensitivity will change, or the type of hold selected when the button is pressed will change.
    * Specifically I don't like the setup menu and the subscreen choice menu. I will be implementing my own ideas for those.
    * Several features and buttons have quirks that will be addressed and behavior that must be documented. For example, the auto-hold feature waits for a "stable" reading, and will re-hold once the measurement changes "significantly". In 88mph, definitions will be established according to the Goals, and the feature will be implemented accordingly, without regard to their definition in the official firmware.
3. Compatible data protocols: The logging format and Bluetooth protocol will be written from scratch, as they are dependent on the internal structure of the official firmware and have several deficiencies.
    * CSV is CSV; as far as I know there are no tools which rely on the exact official format so there is no need to preserve it. The new one will be similar.
    * Bluetooth measurement is tricker. The official format has no opening for improvements or changes. I intend to write PC-compatible scripts to implement the new Bluetooth behavior (such as the Bluetooth console), but I'm not writing more phone apps. Since the Bluetooth apps are entirely Dave's property, maybe I can reach an agreement with him to implement relevant parts of the unofficial protocol in his apps, or to change the official protocol to have room for expansion.

## License
Except where otherwise specified, 88mph is licensed under the Apache 2.0 license according to the notice below.

    Copyright 2018 Thomas Watson

    This file is a part of 88mph: https://github.com/tpwrules/121gw-88mph/

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

STMicroelectronics and ARM hardware libraries are excluded from this license.