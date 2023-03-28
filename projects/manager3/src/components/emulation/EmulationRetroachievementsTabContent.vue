<template>
  <FormFragmentContainer title="emulation.global.retroachievements.title">
    <template v-slot:content>
      <WrappedToggle
        label="emulation.global.retroachievements.activate.label"
        :help="$t('emulation.global.retroachievements.activate.help')"
        :getter="global.retroachievements"
        :setter="globalStore.post"
        apiKey="retroachievements"
        v-if="global.retroachievements"
      />

      <q-separator/>

      <WrappedTextInput
        label="emulation.global.retroachievements.username.label"
        :getter="global['retroachievements.username']"
        :setter="globalStore.post"
        apiKey="retroachievements.username"
        v-if="global['retroachievements.username']"
      />
      <WrappedTextInput
        label="emulation.global.retroachievements.password.label"
        :getter="global['retroachievements.password']"
        :setter="globalStore.post"
        apiKey="retroachievements.password"
        v-if="global['retroachievements.password']"
        password
      />

      <q-separator/>

      <WrappedToggle
        label="emulation.global.retroachievements.hardcore.label"
        :help="$t('emulation.global.retroachievements.hardcore.help')"
        warning
        :getter="global['retroachievements.hardcore']"
        :setter="globalStore.post"
        apiKey="retroachievements.hardcore"
        v-if="global['retroachievements.hardcore']"
      />
    </template>
  </FormFragmentContainer>
</template>

<script lang="ts" setup>
import WrappedTextInput from 'components/global/WrappedTextInput.vue';
import WrappedToggle from 'components/global/WrappedToggle.vue';
import { useGlobalStore } from 'stores/global';
import { storeToRefs } from 'pinia';
import FormFragmentContainer from '../global/FormFragmentContainer.vue';

const globalStore = useGlobalStore();
globalStore.fetch();
const { global } = storeToRefs(globalStore);
</script>

<style lang="sass">

</style>
