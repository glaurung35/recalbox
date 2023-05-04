<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <FormFragmentContainer title="emulation.global.netplay.title">
    <template v-slot:content>
      <WrappedToggle
        label="emulation.global.netplay.activate.label"
        :getter="global.netplay"
        :setter="globalStore.post"
        apiKey="netplay"
        v-if="global.netplay"
        help
      >
        <template v-slot:help>
          {{ $t('emulation.global.netplay.activate.help') }}
        </template>
      </WrappedToggle>
      <WrappedTextInput
        label="emulation.global.netplay.nickname.label"
        warning
        :getter="global['netplay.nickname']"
        :setter="globalStore.post"
        apiKey="netplay.nickname"
        v-if="global['netplay.nickname']"
        help
      >
        <template v-slot:help>
          {{ $t('emulation.global.netplay.nickname.help') }}
        </template>
      </WrappedTextInput>
      <WrappedTextInput
        label="emulation.global.netplay.port.label"
        :getter="global['netplay.port']"
        :setter="globalStore.post"
        apiKey="netplay.port"
        v-if="global['netplay.port']"
        help
      >
        <template v-slot:help>
          {{ $t('emulation.global.netplay.port.help') }}
        </template>
      </WrappedTextInput>
      <WrappedTextInput
        label="emulation.global.netplay.relay.label"
        :getter="global['netplay.relay']"
        :setter="globalStore.post"
        apiKey="netplay.relay"
        v-if="global['netplay.relay']"
        help
      >
        <template v-slot:help>
          {{ $t('emulation.global.netplay.relay.help') }}
        </template>
      </WrappedTextInput>
      <WrappedTextInput
        label="emulation.global.netplay.lobby.label"
        :getter="global['netplay.lobby']"
        :setter="globalStore.post"
        apiKey="netplay.lobby"
        v-if="global['netplay.lobby']"
        help
      >
        <template v-slot:help>
          {{ $t('emulation.global.netplay.lobby.help') }}
        </template>
      </WrappedTextInput>
    </template>
  </FormFragmentContainer>
</template>

<script lang="ts" setup>
import WrappedToggle from 'components/ui-kit/WrappedToggle.vue';
import WrappedTextInput from 'components/ui-kit/WrappedTextInput.vue';
import { useGlobalStore } from 'stores/configuration/global';
import { storeToRefs } from 'pinia';
import FormFragmentContainer from 'components/ui-kit/FormFragmentContainer.vue';

const globalStore = useGlobalStore();
globalStore.fetch();
const { global } = storeToRefs(globalStore);
</script>

<style lang="sass">

</style>
