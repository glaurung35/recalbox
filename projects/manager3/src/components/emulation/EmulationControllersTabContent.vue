<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="emulation.controllers.bluetooth.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.bluetooth.enabled.label"
            :getter="controllers['bluetooth.enabled']"
            :setter="controllersStore.post"
            apiKey="bluetooth.enabled"
            v-if="controllers['bluetooth.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.bluetooth.enabled.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.controllers.bluetooth.ertm.label"
            :getter="controllers['bluetooth.ertm']"
            :setter="controllersStore.post"
            apiKey="bluetooth.ertm"
            v-if="controllers['bluetooth.ertm']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.bluetooth.ertm.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.controllers.bluetooth.autopaironboot.label"
            :getter="controllers['bluetooth.autopaironboot']"
            :setter="controllersStore.post"
            apiKey="bluetooth.autopaironboot"
            v-if="controllers['bluetooth.autopaironboot']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.bluetooth.autopaironboot.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.swapvalidateandcancel.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.swapvalidateandcancel.enabled.label"
            :getter="controllers['swapvalidateandcancel']"
            :setter="controllersStore.post"
            apiKey="swapvalidateandcancel"
            v-if="controllers['swapvalidateandcancel']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.swapvalidateandcancel.enabled.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.osd.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.osd.enabled.label"
            :getter="emulationstation['pads.osd']"
            :setter="emulationstationStore.post"
            apiKey="pads.osd"
            v-if="emulationstation['pads.osd']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.osd.enabled.help') }}
            </template>
          </WrappedToggle>
          <WrappedSelect
            label="emulation.controllers.osd.type.label"
            :options="padsOsdTypeOptions"
            :getter="emulationstation['pads.osd.type']"
            :setter="emulationstationStore.post"
            apiKey="pads.osd.type"
            v-if="emulationstation['pads.osd.type']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.osd.type.help') }}
            </template>
          </WrappedSelect>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.inputdriver.title">
        <template v-slot:content>
          <WrappedSelect
            label="emulation.controllers.inputdriver.label"
            :options="inputdriverOptions"
            :getter="global.inputdriver"
            :setter="globalStore.post"
            apiKey="inputdriver"
            v-if="global.inputdriver"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.inputdriver.help.availableOptions') }}
              <ul>
                <li v-html="$t('emulation.controllers.inputdriver.help.0')"></li>
                <li v-html="$t('emulation.controllers.inputdriver.help.1')"></li>
                <li v-html="$t('emulation.controllers.inputdriver.help.2')"></li>
              </ul>
            </template>
          </WrappedSelect>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.db9.title" v-if="isDB9Available(architecture.arch)">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.db9.enabled.label"
            :getter="controllers['db9.enabled']"
            :setter="controllersStore.post"
            apiKey="db9.enabled"
            v-if="controllers['db9.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.db9.enabled.help') }}
            </template>
          </WrappedToggle>
          <WrappedTextInput
            label="emulation.controllers.db9.settings.label"
            :getter="controllers['db9.args']"
            :setter="controllersStore.post"
            apiKey="db9.args"
            v-if="controllers['db9.args']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.db9.settings.help') }}
            </template>
          </WrappedTextInput>
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="emulation.controllers.gamecon.title" v-if="isGameconAvailable(architecture.arch)">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.gamecon.enabled.label"
            :getter="controllers['gamecon.enabled']"
            :setter="controllersStore.post"
            apiKey="gamecon.enabled"
            v-if="controllers['gamecon.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.gamecon.enabled.help') }}
            </template>
          </WrappedToggle>
          <WrappedTextInput
            label="emulation.controllers.gamecon.settings.label"
            :getter="controllers['gamecon.args']"
            :setter="controllersStore.post"
            apiKey="gamecon.args"
            v-if="controllers['gamecon.args']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.gamecon.settings.help') }}
            </template>
          </WrappedTextInput>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.gpio.title" v-if="isGPIOAvailable(architecture.arch)">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.gpio.enabled.label"
            :getter="controllers['gpio.enabled']"
            :setter="controllersStore.post"
            apiKey="gpio.enabled"
            v-if="controllers['gpio.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.gpio.enabled.help') }}
            </template>
          </WrappedToggle>
          <WrappedTextInput
            label="emulation.controllers.gpio.settings.label"
            :getter="controllers['gpio.args']"
            :setter="controllersStore.post"
            apiKey="gpio.args"
            v-if="controllers['gpio.args']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.gpio.settings.help') }}
            </template>
          </WrappedTextInput>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.ps3.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.ps3.enabled.label"
            :getter="controllers['ps3.enabled']"
            :setter="controllersStore.post"
            apiKey="ps3.enabled"
            v-if="controllers['ps3.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.ps3.enabled.help') }}
            </template>
          </WrappedToggle>
          <WrappedSelect
            label="emulation.controllers.ps3.driver.label"
            :options="ps3driverOptions"
            :getter="controllers['ps3.driver']"
            :setter="controllersStore.post"
            apiKey="ps3.driver"
            v-if="controllers['ps3.driver']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.ps3.driver.help') }}
              <ul>
                <li v-html="$t('emulation.controllers.ps3.driver.0')"></li>
                <li v-html="$t('emulation.controllers.ps3.driver.1')"></li>
                <li v-html="$t('emulation.controllers.ps3.driver.2')"></li>
              </ul>
            </template>
          </WrappedSelect>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.steam.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.steam.enabled.label"
            :getter="controllers['steam.enabled']"
            :setter="controllersStore.post"
            apiKey="steam.enabled"
            v-if="controllers['steam.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.steam.enabled.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.xarcade.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.xarcade.enabled.label"
            :getter="controllers['xarcade.enabled']"
            :setter="controllersStore.post"
            apiKey="xarcade.enabled"
            v-if="controllers['xarcade.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.xarcade.enabled.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.controllers.joycond.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.joycond.enabled.label"
            :getter="controllers['joycond.enabled']"
            :setter="controllersStore.post"
            apiKey="joycond.enabled"
            v-if="controllers['joycond.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.controllers.joycond.enabled.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import FormFragmentContainer from 'components/ui-kit/FormFragmentContainer.vue';
import WrappedSelect from 'components/ui-kit/WrappedSelect.vue';
import WrappedTextInput from 'components/ui-kit/WrappedTextInput.vue';
import WrappedToggle from 'components/ui-kit/WrappedToggle.vue';
import { storeToRefs } from 'pinia';
import { BOARDS } from 'src/utils/constants';
import { useControllersStore } from 'stores/configuration/controllers';
import { useGlobalStore } from 'stores/configuration/global';
import { useEmulationstationStore } from 'stores/configuration/emulationstation';
import { useArchitectureStore } from 'stores/architecture';

const controllersStore = useControllersStore();
controllersStore.fetch();
const { ps3driverOptions, controllers } = storeToRefs(controllersStore);

const globalStore = useGlobalStore();
globalStore.fetch();
const { inputdriverOptions, global } = storeToRefs(globalStore);

const emulationstationStore = useEmulationstationStore();
emulationstationStore.fetch();
const { padsOsdTypeOptions, emulationstation } = storeToRefs(emulationstationStore);

const architectureStore = useArchitectureStore();
architectureStore.fetch();
const { architecture } = storeToRefs(architectureStore);

const isDB9Available = (arch: string) => BOARDS.includes(arch);
const isGameconAvailable = (arch: string) => BOARDS.includes(arch);
const isGPIOAvailable = (arch: string) => BOARDS.includes(arch);
</script>
