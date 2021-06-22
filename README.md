# Phil-f6czv-ESP32_TTGO_T-Display_Digital_Clock
<!-- wp:paragraph -->
<p>In ham-radio digital mode transmissions some protocols (WSPR, FT8, ...) require a synchronization of the order of one second between all stations.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>To have an accurate time on ESP32 there are several solutions: <br> - the connection to an NTP server, <br>- the connection to a GPS, <br>- the use of a saved real-time clock,<br>- a combination of several methods.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>The first two solutions involve either being connected to a network (WiFi, ...) or to be able to receive GPS signals which is not always possible.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>This article presents the realization of a date-time display on an ESP32 TTGO T-Display using a saved DS3231 real-time clock.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>The principle is as follows:</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>- the date and time data is acquired by a WiFi connection with an NTP server and saved on the DS3231, <br> - the DS3231 then provides the data for display.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>Two software versions were developped, one using a button of the TTGO T-Display to switch on or off the display.
    <br>73 Philippe F6CZV
      https://f6czv.fr</p>
<!-- /wp:paragraph -->
