<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="emulation.controllers.bluetooth.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.bluetooth.enabled"
            :getter="controllers['bluetooth.enabled']"
            :setter="controllersStore.post"
            apiKey="bluetooth.enabled"
            v-if="controllers['bluetooth.enabled']"
          />
          <WrappedToggle
            label="emulation.controllers.bluetooth.ertm"
            :getter="controllers['bluetooth.ertm']"
            :setter="controllersStore.post"
            apiKey="bluetooth.ertm"
            v-if="controllers['bluetooth.ertm']"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.db9.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.db9.enabled"
            :getter="controllers['db9.enabled']"
            :setter="controllersStore.post"
            apiKey="db9.enabled"
            v-if="controllers['db9.enabled']"
          />
          <WrappedTextInput
            label="emulation.controllers.db9.settings"
            :getter="controllers['db9.args']"
            :setter="controllersStore.post"
            apiKey="db9.args"
            v-if="controllers['db9.args']"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.gamecon.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.gamecon.enabled"
            :getter="controllers['gamecon.enabled']"
            :setter="controllersStore.post"
            apiKey="gamecon.enabled"
            v-if="controllers['gamecon.enabled']"
          />
          <WrappedTextInput
            label="emulation.controllers.gamecon.settings"
            :getter="controllers['gamecon.args']"
            :setter="controllersStore.post"
            apiKey="gamecon.args"
            v-if="controllers['gamecon.args']"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.gpio.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.gpio.enabled"
            :getter="controllers['gpio.enabled']"
            :setter="controllersStore.post"
            apiKey="gpio.enabled"
            v-if="controllers['gpio.enabled']"
          />
          <WrappedTextInput
            label="emulation.controllers.gpio.settings"
            :getter="controllers['gpio.args']"
            :setter="controllersStore.post"
            apiKey="gpio.args"
            v-if="controllers['gpio.args']"
          />
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="emulation.controllers.ps3.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.ps3.enabled"
            :getter="controllers['ps3.enabled']"
            :setter="controllersStore.post"
            apiKey="ps3.enabled"
            v-if="controllers['ps3.enabled']"
          />
          <WrappedSelect
            label="emulation.controllers.ps3.label"
            :options="ps3driverOptions"
            :getter="controllers['ps3.driver']"
            :setter="controllersStore.post"
            apiKey="ps3.driver"
            v-if="controllers['ps3.driver']"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.steam.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.steam.enabled"
            :getter="controllers['steam.enabled']"
            :setter="controllersStore.post"
            apiKey="steam.enabled"
            v-if="controllers['steam.enabled']"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.xarcade.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.xarcade.enabled"
            :getter="controllers['xarcade.enabled']"
            :setter="controllersStore.post"
            apiKey="xarcade.enabled"
            v-if="controllers['xarcade.enabled']"
          />
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import FormFragmentContainer from 'components/global/FormFragmentContainer.vue';
import WrappedSelect from 'components/global/WrappedSelect.vue';
import WrappedTextInput from 'components/global/WrappedTextInput.vue';
import WrappedToggle from 'components/global/WrappedToggle.vue';
import { storeToRefs } from 'pinia';
import { useControllersStore } from 'stores/configuration/controllers';

const controllersStore = useControllersStore();
controllersStore.fetch();
const { ps3driverOptions, controllers } = storeToRefs(controllersStore);
</script>
