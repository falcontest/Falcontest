# Makefile for scripts
#
# This file is part of DRBD by Philipp Reisner & Lars Ellenberg.
#
# Copright 2001-2008 LINBIT Information Technologies
# Philipp Reisner, Lars Ellenberg
#
# drbd is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# drbd is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with drbd; see the file COPYING.  If not, write to
# the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
#
# for Debian:
#   update-rc.d drbd defaults 70 08
#

# variables set by configure
DISTRO = redhat
prefix = /usr/local
exec_prefix = /usr/local
datarootdir = /usr/local/share
datadir = /usr/local/share
sbindir = /usr/local/sbin
sysconfdir = /usr/local/etc
BASH_COMPLETION_SUFFIX = 
UDEV_RULE_SUFFIX = 
INITDIR = /usr/local/etc/rc.d/init.d
LIBDIR = /usr/local/lib/drbd
LN_S = ln -s
INITSCRIPT_SYMLINK = 

# features enabled or disabled by configure
WITH_UTILS = yes
WITH_KM = no
WITH_UDEV = yes
WITH_XEN = yes
WITH_PACEMAKER = yes
WITH_HEARTBEAT = yes
WITH_RGMANAGER = no
WITH_BASHCOMPLETION = yes

# variables meant to be overridden from the make command line
DESTDIR ?= /

all:

install: install-utils install-udev install-xen install-heartbeat install-pacemaker install-rgmanager install-bashcompletion

install-utils:
ifeq ($(WITH_UTILS),yes)
	install -d $(DESTDIR)$(INITDIR)
	install -m 755 drbd $(DESTDIR)$(INITDIR)/
	@ if [ ! -e $(DESTDIR)$(sysconfdir)/drbd.conf ]; then \
		install -d $(DESTDIR)$(sysconfdir)/; \
		install -m 644 drbd.conf $(DESTDIR)$(sysconfdir)/; \
		install -d $(DESTDIR)$(sysconfdir)/drbd.d; \
		install -m 644 global_common.conf $(DESTDIR)$(sysconfdir)/drbd.d; \
	fi
	install -d $(DESTDIR)$(LIBDIR)
	install -m 755 outdate-peer.sh $(DESTDIR)$(LIBDIR)
	install -m 755 snapshot-resync-target-lvm.sh $(DESTDIR)$(LIBDIR)
	install -m 755 notify.sh $(DESTDIR)$(LIBDIR)
	( set -e ; cd $(DESTDIR)$(LIBDIR) ;\
	  $(LN_S) -f snapshot-resync-target-lvm.sh unsnapshot-resync-target-lvm.sh ;\
	  $(LN_S) -f notify.sh notify-split-brain.sh ;\
	  $(LN_S) -f notify.sh notify-io-error.sh ;\
	  $(LN_S) -f notify.sh notify-pri-on-incon-degr.sh ;\
	  $(LN_S) -f notify.sh notify-pri-lost.sh ;\
	  $(LN_S) -f notify.sh notify-pri-lost-after-sb.sh ;\
	  $(LN_S) -f notify.sh notify-emergency-reboot.sh ;\
	  $(LN_S) -f notify.sh notify-emergency-shutdown.sh ;\
	  $(LN_S) -f notify.sh notify-out-of-sync.sh; )
	install -d $(DESTDIR)$(sbindir)
	install -m 755 drbd-overview.pl $(DESTDIR)$(sbindir)/drbd-overview
ifeq ($(DISTRO),suse)
	$(LN_S) -f ..$(sysconfdir)/init.d/drbd $(DESTDIR)$(INITSCRIPT_SYMLINK)
endif
ifeq ($(DISTRO),debian)
	@ echo "Don't forget to run update-rc.d"
else
	@ echo "Don't forget to run chkconfig"
endif
endif

install-heartbeat:
ifeq ($(WITH_HEARTBEAT),yes)
	mkdir -p $(DESTDIR)$(sysconfdir)/ha.d/resource.d
	install -m 755 drbddisk $(DESTDIR)$(sysconfdir)/ha.d/resource.d
	install -m 755 drbdupper $(DESTDIR)$(sysconfdir)/ha.d/resource.d
endif

# Do not use $(prefix) for the resource agent. The OCF standard
# explicitly mandates where resource agents must live,
# no matter what prefix is configured to.
install-pacemaker:
ifeq ($(WITH_PACEMAKER),yes)
	install -d $(DESTDIR)$(LIBDIR)
	install -m 755 crm-fence-peer.sh $(DESTDIR)$(LIBDIR)
	( set -e ; cd $(DESTDIR)$(LIBDIR) ;\
	  $(LN_S) -f crm-fence-peer.sh crm-unfence-peer.sh ; )
	mkdir -p $(DESTDIR)/usr/lib/ocf/resource.d/linbit
	install -m 755 drbd.ocf $(DESTDIR)/usr/lib/ocf/resource.d/linbit/drbd
endif

install-rgmanager:
ifeq ($(WITH_RGMANAGER),yes)
	mkdir -p $(DESTDIR)$(datadir)/cluster
	install -m 755 drbd.sh.rhcs $(DESTDIR)$(datadir)/cluster/drbd.sh
	install -m 644 drbd.metadata.rhcs $(DESTDIR)$(datadir)/cluster/drbd.metadata
endif

install-xen:
ifeq ($(WITH_XEN),yes)
	mkdir -p $(DESTDIR)$(sysconfdir)/xen/scripts
	install -m 755 block-drbd $(DESTDIR)$(sysconfdir)/xen/scripts
endif

install-udev:
ifeq ($(WITH_UDEV),yes)
	mkdir -p $(DESTDIR)$(sysconfdir)/udev/rules.d
	install -m 644 drbd.rules $(DESTDIR)$(sysconfdir)/udev/rules.d/65-drbd.rules$(UDEV_RULE_SUFFIX)
endif

install-bashcompletion:
ifeq ($(WITH_BASHCOMPLETION),yes)
	mkdir -p $(DESTDIR)$(sysconfdir)/bash_completion.d
	install -m 644 drbdadm.bash_completion $(DESTDIR)$(sysconfdir)/bash_completion.d/drbdadm$(BASH_COMPLETION_SUFFIX)
endif

clean:
	rm -f *~
	rm -f datadisk

distclean: clean

uninstall:
	rm -f $(DESTDIR)$(INITDIR)/drbd
	rm -f $(DESTDIR)$(sysconfdir)/ha.d/resource.d/drbddisk
	rm -f $(DESTDIR)$(sysconfdir)/ha.d/resource.d/drbdupper
	rm -f $(DESTDIR)$(sysconfdir)/xen/scripts/block-drbd
	rm -f $(DESTDIR)$(sysconfdir)/bash_completion.d/$(COMPLETION)
	! test -L $(DESTDIR)/sbin/rcdrbd || rm $(DESTDIR)/sbin/rcdrbd
