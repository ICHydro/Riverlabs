---
title: Installation
tags:
keywords:
summary: "This page covers technical aspects of installing distance sensors for non-contact river stage monitoring"
sidebar: home_sidebar
permalink: installation.html
---

## Wari logger (ultrasound sensor)

The Wari logger uses a Maxbotix ultrasonic distance sensors, with a maximum theoretical range of 5m. However, in practice the range may be slightly lower, especially for surfaces with low reflectivity. Most water surfaces reflect well but it is recommended to build in a margin of around 0.5m.

Although ultrasonic sensors may be able to measure at an angle, this may induce errors as a result of the diverging beam of the ultrasound signal. This divergence also means that the sensor needs to be installed at a sufficient distance of other obstacles that may reflect the signal, such as the river bank. The technical specifications of the sensor provides further details, but in general we recommend a horizontal distance of minimum 0.2&nbsp;m between the travel path of the signal and any obstacle:

![Ultrasound installation sketch](images/installation_US.png "Wari installation")


## WMOnode logger (lidar sensor)

The lidar based WMO logger has the unique advantage of being able to measure at angles up to 40&deg; with minimal loss of accuracy, because of the very low divergence of the lidar beam. The theoretical range of the lidar is 35m, but this is more dependent on the reflectivity of the water surface. This reflectivity is lowest for a perfectly smooth water surface and fully transparent water. Any surface roughness or impurities in the water (such as sediment) will increase the reflectivity strongly. In natural river conditions, the reflectivity is typically sufficiently high to warrant good performance:

![Lidar installation sketch](images/installation_lidar.png "WMOnode installation")

