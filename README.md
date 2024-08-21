<div id="top"></div>

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]


<!-- PROJECT LOGO -->
<br />
<div align="center">
	<a href="https://github.com/MoonGrt/Cruise_Robot">
	<img src="images/logo.png" alt="Logo" width="80" height="80">
	</a>
<h3 align="center">Cruise_Robot</h3>
	<p align="center">
	This project involves the development of a cruise robot based on the RT-Thread (Art-Pi) and ROS platforms, capable of real-time SLAM and autonomous navigation. The robot utilizes RT-Thread's multithreading to drive a LiDAR, odometer, and inertial module to gather current data, which is then processed using ROS's Cartographer for localization and map building, and Nav2 for path planning and obstacle avoidance. Additionally, the robot is equipped with the Yolo-Fastest model for real-time target detection, making it highly effective in identifying and tracking objects in its environment. 
	<br />
	<a href="https://github.com/MoonGrt/Cruise_Robot"><strong>Explore the docs »</strong></a>
	<br />
	<br />
	<a href="https://github.com/MoonGrt/Cruise_Robot">View Demo</a>
	·
	<a href="https://github.com/MoonGrt/Cruise_Robot/issues">Report Bug</a>
	·
	<a href="https://github.com/MoonGrt/Cruise_Robot/issues">Request Feature</a>
	</p>
</div>


<!-- CONTENTS -->
<details open>
  <summary>Contents</summary>
  <ol>
    <li><a href="#file-tree">File Tree</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>


<!-- FILE TREE -->
## File Tree

```
└─ Project
  ├─ LICENSE
  ├─ README.md
  ├─ /Project/
  │ └─ /applications/
  │   ├─ SConscript
  │   ├─ YDLIDAR.c
  │   ├─ YDLIDAR2.c
  │   ├─ YDLIDAR3.c
  │   ├─ ai.c
  │   ├─ camera.c
  │   ├─ dht11.c
  │   ├─ encoder.c
  │   ├─ ft6236.c
  │   ├─ main.c
  │   ├─ motor.c
  │   ├─ motor2.c
  │   ├─ mpu6050.c
  │   ├─ onenet.c
  │   ├─ onenet_dht11.c
  │   ├─ ros_car.c
  │   ├─ ros_image.c
  │   ├─ ros_pub.c
  │   ├─ ros_pub2.c
  │   ├─ ros_subint.c
  │   ├─ rt_ai_person_yolo_model.c
  │   ├─ rt_ai_person_yolo_model.h
  │   ├─ sensor_dallas_dht11.c
  │   ├─ sensor_dallas_dht11.h
  │   ├─ timer.c
  │   ├─ yolo_layer.c
  │   ├─ yolo_layer.h
  │   └─ /web_dist/
  │     └─ /webnet/
  │       └─ index.html
  └─ /images/

```


<!-- CONTRIBUTING -->
## Contributing
Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.
If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!
1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request
<p align="right">(<a href="#top">top</a>)</p>


<!-- LICENSE -->
## License
Distributed under the MIT License. See `LICENSE` for more information.
<p align="right">(<a href="#top">top</a>)</p>


<!-- CONTACT -->
## Contact
MoonGrt - 1561145394@qq.com
Project Link: [MoonGrt/](https://github.com/MoonGrt/)
<p align="right">(<a href="#top">top</a>)</p>


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments
* [Choose an Open Source License](https://choosealicense.com)
* [GitHub Emoji Cheat Sheet](https://www.webpagefx.com/tools/emoji-cheat-sheet)
* [Malven's Flexbox Cheatsheet](https://flexbox.malven.co/)
* [Malven's Grid Cheatsheet](https://grid.malven.co/)
* [Img Shields](https://shields.io)
* [GitHub Pages](https://pages.github.com)
* [Font Awesome](https://fontawesome.com)
* [React Icons](https://react-icons.github.io/react-icons/search)   
<p align="right">(<a href="#top">top</a>)</p>


<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/MoonGrt/Cruise_Robot.svg?style=for-the-badge
[contributors-url]: https://github.com/MoonGrt/Cruise_Robot/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/MoonGrt/Cruise_Robot.svg?style=for-the-badge
[forks-url]: https://github.com/MoonGrt/Cruise_Robot/network/members
[stars-shield]: https://img.shields.io/github/stars/MoonGrt/Cruise_Robot.svg?style=for-the-badge
[stars-url]: https://github.com/MoonGrt/Cruise_Robot/stargazers
[issues-shield]: https://img.shields.io/github/issues/MoonGrt/Cruise_Robot.svg?style=for-the-badge
[issues-url]: https://github.com/MoonGrt/Cruise_Robot/issues
[license-shield]: https://img.shields.io/github/license/MoonGrt/Cruise_Robot.svg?style=for-the-badge
[license-url]: https://github.com/MoonGrt/Cruise_Robot/blob/master/LICENSE

