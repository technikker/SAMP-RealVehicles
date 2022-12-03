stock log_vehicle(str[])
{
	new filename[256];
	format(filename, sizeof(filename), "%s%s", VEHICLE_LOGS_LOCATION, VEHICLE_LOG_NAME);

	new File:log_file;
	log_file = fopen(filename, io_append); // Open the file
	if(!log_file) return 0;

	new log_output[256];
	format(log_output, sizeof(log_output), "%s\r\n", str);

	fwrite(log_file, log_output); // Write to the file (\r\n is for a new line)
	fclose(log_file);

	return 1;
}