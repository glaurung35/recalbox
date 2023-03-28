<template>
  <FormFragmentContainer title="emulation.global.netplay.title">
    <template v-slot:content>
      <WrappedToggle
        label="emulation.global.netplay.activate.label"
        :help="$t('emulation.global.netplay.activate.help')"
        :getter="global.netplay"
        :setter="globalStore.post"
        apiKey="netplay"
        v-if="global.netplay"
      />
      <WrappedTextInput
        label="emulation.global.netplay.nickname.label"
        :help="$t('emulation.global.netplay.nickname.help')"
        warning
        :getter="global['netplay.nickname']"
        :setter="globalStore.post"
        apiKey="netplay.nickname"
        v-if="global['netplay.nickname']"
      />
      <WrappedTextInput
        label="emulation.global.netplay.port.label"
        :help="$t('emulation.global.netplay.port.help')"
        :getter="global['netplay.port']"
        :setter="globalStore.post"
        apiKey="netplay.port"
        v-if="global['netplay.port']"
      />
      <WrappedTextInput
        label="emulation.global.netplay.relay.label"
        :help="$t('emulation.global.netplay.relay.help')"
        :getter="global['netplay.relay']"
        :setter="globalStore.post"
        apiKey="netplay.relay"
        v-if="global['netplay.relay']"
      />
      <WrappedTextInput
        label="emulation.global.netplay.lobby.label"
        :help="$t('emulation.global.netplay.lobby.help')"
        :getter="global['netplay.lobby']"
        :setter="globalStore.post"
        apiKey="netplay.lobby"
        v-if="global['netplay.lobby']"
      />
    </template>
  </FormFragmentContainer>
</template>

<script lang="ts" setup>
import WrappedToggle from 'components/global/WrappedToggle.vue';
import WrappedTextInput from 'components/global/WrappedTextInput.vue';
import { useGlobalStore } from 'stores/global';
import { storeToRefs } from 'pinia';
import FormFragmentContainer from '../global/FormFragmentContainer.vue';

const globalStore = useGlobalStore();
globalStore.fetch();
const { global } = storeToRefs(globalStore);
</script>

<style lang="sass">

</style>
