# Third-Party Notices for qcm_mqtt

The `qcm_mqtt` component was developed with reference to the Eclipse Paho MQTT
Embedded C project. As a conservative compliance measure, this component
distribution includes the upstream license texts for source files or code
fragments that were copied from, adapted from, or substantially derived from
Eclipse Paho MQTT Embedded C.

Upstream project: Eclipse Paho MQTT Embedded C
Upstream site: https://github.com/eclipse-paho/paho.mqtt.embedded-c
Reference code line reviewed: 1.1.0
Upstream license options: Eclipse Distribution License v1.0 or Eclipse Public
License v1.0
Selected license for this distribution: Eclipse Distribution License v1.0

Primary upstream copyright notice:

Copyright (c) 2014, 2017 IBM Corp.

Files in this component that currently carry upstream provenance notices:

- `qcm_mqtt_packet.c`
- `qcm_mqtt_packet.h`
- `qcm_mqtt_private.h`

The selected upstream license text is provided in:

- `licenses/paho_edl_v10.txt`

Maintenance rules for future changes:

1. If a `qcm_mqtt` source file contains code copied or directly adapted from
   Eclipse Paho MQTT Embedded C, keep the upstream provenance notice in that
   file header, or add an equivalent provenance note in the file header.
2. When exporting or redistributing `qcm_mqtt` source or SDK packages, include
   this notice file and the corresponding EDL license file under `licenses/`.
3. Do not use the names of the Eclipse Foundation, IBM, or upstream
   contributors to endorse or promote derived products without prior written
   permission.
