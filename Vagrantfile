Vagrant.configure("2") do |config|
    config.vm.box = "debian/contrib-buster64"
    config.vm.synced_folder ".", "/vagrant"
    config.vm.provision "ansible_local" do |ansible|
      ansible.playbook = "ansible/vagrant.yml"
    end

    config.vm.provider :virtualbox do |vb|
        # distinguish VMs by a location-dependent suffix
        name_suffix = Digest::SHA1.hexdigest(Dir.pwd)[0..6]

        vb.gui = true
        vb.memory = 2048
        vb.name = "Artery Vagrant VM " + name_suffix
        vb.customize ["modifyvm", :id, "--vram", "32"]
    end

    config.vm.provider "hyperv" do |h, override|
        # box "debian/contrib-buster64" is not supported by hyperv
        # use a different
        override.vm.box = "generic/debian10"

        # distinguish VMs by a location-dependent suffix
        name_suffix = Digest::SHA1.hexdigest(Dir.pwd)[0..6]

        h.vmname = "Artery Vagrant VM " + name_suffix
        h.memory = 2048
        h.vm_integration_services = {
          guest_service_interface: true,
          heartbeat: true,
          key_value_pair_exchange: true,
          shutdown: true,
          time_synchronization: true,
          vss: true
        }
    end
end
