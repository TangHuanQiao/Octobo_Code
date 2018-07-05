deps_config := \
	/home/Administrator/esp/esp-idf/components/app_trace/Kconfig \
	/home/Administrator/esp/esp-idf/components/aws_iot/Kconfig \
	/home/Administrator/esp/esp-idf/components/bt/Kconfig \
	/home/Administrator/esp/esp-idf/components/driver/Kconfig \
	/home/Administrator/esp/esp-idf/components/esp32/Kconfig \
	/home/Administrator/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/Administrator/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/Administrator/esp/esp-idf/components/ethernet/Kconfig \
	/home/Administrator/esp/esp-idf/components/fatfs/Kconfig \
	/home/Administrator/esp/esp-idf/components/freertos/Kconfig \
	/home/Administrator/esp/esp-idf/components/heap/Kconfig \
	/home/Administrator/esp/esp-idf/components/libsodium/Kconfig \
	/home/Administrator/esp/esp-idf/components/log/Kconfig \
	/home/Administrator/esp/esp-idf/components/lwip/Kconfig \
	/home/Administrator/esp/esp-idf/components/mbedtls/Kconfig \
	/home/Administrator/esp/esp-idf/components/mdns/Kconfig \
	/home/Administrator/esp/esp-idf/components/openssl/Kconfig \
	/home/Administrator/esp/esp-idf/components/pthread/Kconfig \
	/home/Administrator/esp/esp-idf/components/spi_flash/Kconfig \
	/home/Administrator/esp/esp-idf/components/spiffs/Kconfig \
	/home/Administrator/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/Administrator/esp/esp-idf/components/vfs/Kconfig \
	/home/Administrator/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/Administrator/esp/esp-idf/Kconfig.compiler \
	/home/Administrator/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/Administrator/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/Administrator/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/Administrator/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
