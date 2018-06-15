#sudo apt-get install g++ binutils libc6-dev-i386 grub-legacy

GCCPARAMS = -m32 -Ialinc -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
	  obj/gdt.o \
	  obj/drivers/driver.o \
	  obj/hwcom/port.o \
	  obj/hwcom/interruptstubs.o \
	  obj/hwcom/interrupts.o \
	  obj/hwcom/pci.o \
	  obj/drivers/keyboard.o \
	  obj/drivers/mouse.o \
	  obj/kernel.o

obj/%.o: alsrc/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -o $@ -c $<

obj/%.o: alsrc/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

aseakk.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: aseakk.bin
	sudo cp $< /boot/aseakk.bin

asea.iso: aseakk.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot
	echo 'set timeout=10' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Asea OS" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/aseakk.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

.PHONY: clean
clean:
	rm -rf obj aseakk.bin asea.iso
