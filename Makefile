# This file is part of Asea OS.
# Copyright (C) 2018 - 2020 Ivan Kmeťo
#
# Asea OS is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# Asea OS is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along with
# Asea OS.  If not, see <http://www.gnu.org/licenses/>.


GXX = /usr/bin/g++
GXXFLAGS = -m32 -std=c++11 -Ialinc -Ilib -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASFLAGS = --32
LDFLAGS = -melf_i386
KERNEL = al
VINAME = 0.13-testing200729

objects = obj/loader.o \
	  obj/gdt.o \
	  obj/memmgr.o \
	  obj/drivers/driver.o \
	  obj/hwcom/port.o \
	  obj/hwcom/interruptstubs.o \
	  obj/hwcom/interrupts.o \
	  obj/hwcom/pci.o \
	  obj/drivers/keyboard.o \
	  obj/drivers/mouse.o \
	  obj/drivers/vga.o \
	  obj/kernel.o

obj/%.o: alsrc/%.cpp
	mkdir -p $(@D)
	$(GXX) $(GXXFLAGS) -o $@ -c $<

obj/%.o: alsrc/%.s
	mkdir -p $(@D)
	as $(ASFLAGS) -o $@ $<

asea.al.bin: linker.ld $(objects)
	ld $(LDFLAGS) -T $< -o $@ $(objects)

iso: asea.al.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot
	cp LICENSE iso
	echo 'set timeout=10' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Asea AL" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/asea.al.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=asea-$(KERNEL)-$(VINAME).iso iso
	rm -rf iso

.PHONY: clean checkdeps installdeps install
clean:
	rm -rf obj asea.$(KERNEL).bin asea-$(KERNEL)-$(VINAME).iso

checkdeps:
	@echo "\033[1;33m[1/8] GNU Make\033[0m"
	@echo "\033[1;36mRecommended GNU Make 4.2.1\033[0m"
	@make --version
	@echo "\033[1;33m[2/8] G++\033[0m"
	@echo "\033[1;36mRecommended G++ 8.3.0\033[0m"
	@g++ --version
	@echo "\033[1;33m[3/8] Binutils: GNU linker\033[0m"
	@echo "\033[1;36mRecommended GNU Binutils 2.31.1\033[0m"
	@ld --version
	@echo "\n"
	@echo "\033[1;33m[4/8] Binutils: GNU assembler\033[0m"
	@echo "\033[1;36mRecommended GNU Binutils 2.31.1\033[0m"
	@as --version
	@echo "\n"
	@echo "\033[1;33m[5/8] libc6-dev-i386\033[0m"
	@echo "\033[1;36mRecommended libc6-dev-i386 2.28-10 \033[0m"
	@dpkg -l | grep libc6-dev-i386
	@echo "\n"
	@echo "\033[1;33m[6/8] GRUB Legacy\033[0m"
	@echo "\033[1;36mRecommended grub-legacy 0.97-75 \033[0m"
	@dpkg -l | grep grub-legacy
	@echo "\n"
	@echo "\033[1;33m[7/8] grub-pc-bin\033[0m"
	@echo "\033[1;36mRecommended grub-pc-bin 2.02+dfsg1-20 \033[0m"
	@dpkg -l | grep grub-pc-bin
	@echo "\n"
	@echo "\033[1;33m[8/8] xorriso\033[0m"
	@echo "\033[1;36mRecommended xorriso 1.5.0 \033[0m"
	@xorriso --version

installdeps:
	@echo "\033[1;33m[1/6] Check & Install: G++\033[0m"
	@sudo apt-get install g++
	@echo "\033[1;33m[2/6] Check & Install: Binutils\033[0m"
	@sudo apt-get install binutils
	@echo "\033[1;33m[3/6] Check & Install: libc6-dev-i386\033[0m"
	@sudo apt-get install libc6-dev-i386
	@echo "\033[1;33m[4/6] Check & Install: GRUB Legacy\033[0m"
	@sudo apt-get install grub-legacy
	@echo "\033[1;33m[5/6] Check & Install: grub-pc-bin\033[0m"
	@sudo apt-get install grub-pc-bin
	@echo "\033[1;33m[6/6] Check & Install: xorriso\033[0m"
	@sudo apt-get install xorriso
