# Data Download

This guide will walk you through downloading data from your Riverlabs logger for the first time.

## Before You Start

Your logger stores data on a microSD card. To download data, you'll need:

- **MicroSD card reader** (USB or built into your computer)
- Access to the logger to remove the SD card
- A computer with spreadsheet software (Excel, Google Sheets, etc.)

## Step 1: Safely Remove the SD Card

!!! warning "Always flush data before removal"
    Follow these steps to prevent data loss:

1. Press the **RESET** button on the logger to flush any buffered data
2. Wait for the LED to show a red pulse (indicating data transfer)
3. Once the LED turns off, the data has been flushed to the microSD card.
4. Gently push and release the SD card to eject it from the slot
5. Remove the SD card carefully

!!! tip
    The flushing process may take several seconds depending on the amount of buffered data. Never remove the SD card while the LED is illuminated.

## Step 2: Insert SD Card into Your Computer

1. Insert the microSD card into your card reader
2. Connect the card reader to your computer (if external)
3. Wait for your computer to recognize the SD card
4. Open your file explorer/finder to locate the SD card drive

## Step 3: Locate Your Data Files

The logger creates daily data files with the naming format: **YYYYMMDD.CSV**

For example:
- `20260103.CSV` - Data from January 3, 2026
- `20260104.CSV` - Data from January 4, 2026

Navigate to the SD card and you'll see one CSV file for each day the logger collected data.

## Step 4: Copy the Data Files

1. Select the data files you want to download
2. Copy them to a folder on your computer
3. We recommend organizing by deployment or location:
   ```
   My Documents/
   └── Logger Data/
       └── Site A - Jan 2026/
           ├── 20260101.CSV
           ├── 20260102.CSV
           └── 20260103.CSV
   ```

!!! tip "Best Practice"
    Always copy files rather than moving them, so you maintain a backup on the SD card until you're certain the data is safely stored.

## Step 5: Open and View Your Data

The CSV files can be opened with any spreadsheet software:

1. Open your spreadsheet application (Excel, Google Sheets, etc.)
2. Open the CSV file
3. You should see columns with your data

### Understanding the Data Format

Each line represents one measurement period with the following columns:

| Column | Content | Example | Description |
|--------|---------|---------|-------------|
| 1 | Date/Time | 2026/01/03 12:00:00 | Timestamp in YYYY/MM/DD HH:MM:SS format |
| 2-11 | Distance readings | 2215, 2214, 2214... | 10 consecutive raw measurements in millimeters |
| 12 | Battery voltage | 4100 | Battery voltage in millivolts (mV) |
| 13 | Temperature | 1950 | Logger temperature in 1/100°C (1950 = 19.50°C) |

**Example row:**
```
2026/01/03 12:00:00, 2215, 2214, 2214, 2215, 2214, 2214, 2214, 2214, 2215, 2215, 4100, 1950
```

## Step 6: Return the SD Card

Once you've confirmed your data has been successfully copied:

1. Safely eject the SD card from your computer
2. Reinsert it into the logger:
   - Ensure correct orientation (contacts facing outward, away from battery)
   - Push gently until it clicks into place
3. Press the **RESET** button to reinitialize the logger


## Troubleshooting

**SD card not recognized:**
- Try a different card reader
- Check if card needs formatting (FAT or FAT32)
- Verify card is not write-protected

**No data files on card:**
- Logger may not have been initialized properly
- Check if logger is taking measurements (LED should flash periodically)
- Verify clock is set correctly

**Corrupted or incomplete files:**
- May indicate SD card was removed without flushing data
- Always press RESET and wait for LED to finish before removing card
- Consider replacing the SD card if problems persist

## Next Steps

Now that you've successfully downloaded your first data:

- [Data Handling](../../data-handling.md) - Detailed SD card management
- [Common Issues](../troubleshooting/common-issues.md) - Troubleshooting data problems
- [Maintenance Guide](../hardware/maintenance.md) - Regular maintenance schedule
- [ThingsBoard Configuration](../telemetry/thingsboard-configuration.md) - Set up remote monitoring

## Quick Reference Card

| Action | Steps |
|--------|-------|
| **Remove SD card** | Press RESET → Wait for LED → Remove card |
| **View data** | Open .CSV file in Excel/Sheets |
| **Data columns** | DateTime, 10x Distance, Voltage, Temperature |
| **Reinsert card** | Insert card → Press RESET |
| **File naming** | YYYYMMDD.CSV (e.g., 20260103.CSV) |
