# Summary

Berkey is gravity fed. It's a pain to refill so let's automate it.

Challenges:
- It takes time to filter so have to delay continuing filling the top
- The etape sensor is tricky, especially when it overflows
- Water pump is very strong on and off, could be loud for the pumps

# Todos

- [x] Are the Etape sensors stable?
- [x] Figure out the right cable orientation and document
- [x] Test proper sensor input with `Serial.println()`.
- [x] Make percentage for sensor work so it's easier to work with
- [x] Water pump working
- [x] Implement algo


## How the E-Tape Works

The ETape Sensor:
- Black $Grd$
- Red - $Vin$ ($6 – 30VDC$)
- White $Vout$ ($0V - 5V$)

[ETape DataSheet](https://img1.wsimg.com/blobby/go/6e1bce17-f4fa-40c3-9d89-9bb7445697bb/downloads/0-5V%20Module%20Data%20Sheet.pdf)

Arduino's analog pins take a reading between $0V$ and $5V$ 

Ranges of the sensors seem to be different from one sensor to the next.

# Documentation

Cable connection: Etape Black -> Cable Red -> Grnd
This uses the Arduino Nano Every

Failsafes:
* If it tries to fill over 20 times in one attempt, then the system shutdown. Likely the sensor isn't working on the filling itself isn't adding water.

## Algorithm

![](images/algorithm.png)
