<!DOCTYPE busconfig PUBLIC
 "-//freedesktop//DTD D-BUS Bus Configuration 1.0//EN"
 "http://www.freedesktop.org/standards/dbus/1.0/busconfig.dtd">
<busconfig>
  <policy user="@CAGIBI_DAEMON_SYSTEMUSER@">
    <allow own="org.kde.Cagibi"/>
  </policy>
  <policy user="root">
    <allow own="org.kde.Cagibi"/>
  </policy>
  <policy context="default">
    <allow send_destination="org.kde.Cagibi"/>
    <allow receive_sender="org.kde.Cagibi"/>
    <deny send_destination="org.kde.Cagibi"
           send_interface="org.kde.Cagibi.Control"/>
  </policy>
  <policy group="@CAGIBI_DAEMON_SYSTEMUSER@">
    <allow send_destination="org.kde.Cagibi"/>
    <allow receive_sender="org.kde.Cagibi"/>
  </policy>
  <policy user="root">
    <allow send_destination="org.kde.Cagibi"/>
    <allow receive_sender="org.kde.Cagibi"/>
  </policy>
</busconfig>
