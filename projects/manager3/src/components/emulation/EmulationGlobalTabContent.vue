<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="emulation.global.ratio.title">
        <template v-slot:content>
          <WrappedSelect
            label="emulation.global.ratio.label"
            :options="ratioOptions"
            :getter="global.ratio"
            :setter="globalStore.post"
            apiKey="ratio"
            v-if="global.ratio"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.global.rewind.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.global.rewind.label"
            :help="$t('emulation.global.rewind.warning')"
            warning
            :getter="global.rewind"
            :setter="globalStore.post"
            apiKey="rewind"
            v-if="global.rewind"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.global.autosave.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.global.autosave.label"
            :help="$t('emulation.global.autosave.help')"
            :getter="global.autosave"
            :setter="globalStore.post"
            apiKey="autosave"
            v-if="global.autosave"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.global.shaders.title">
        <template v-slot:content>
          <WrappedSelect
            label="emulation.global.shaders.label"
            :help="$t('emulation.global.shaders.help')"
            :options="shadersetOptions"
            :getter="global.shaderset"
            :setter="globalStore.post"
            apiKey="shaderset"
            v-if="global.shaderset"
          />
          <WrappedSelect
            label="emulation.global.customshaders.label"
            :options="shadersetfileOptions"
            :getter="global['shaderset.file']"
            :setter="globalStore.post"
            apiKey="shaderset.file"
            v-if="global['shaderset.file']"
            :disable="global.shaderset.value !== 'custom'"
          />
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="emulation.global.integerscale.title">
        <template v-slot:content>
            <WrappedToggle
              label="emulation.global.integerscale.label"
              :help="$t('emulation.global.integerscale.help')"
              :getter="global.integerscale"
              :setter="globalStore.post"
              apiKey="integerscale"
              v-if="global.integerscale"
            />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.global.smooth.title">
        <template v-slot:content>
            <WrappedToggle
              label="emulation.global.smooth.label"
              :getter="global.smooth"
              :setter="globalStore.post"
              apiKey="smooth"
              v-if="global.smooth"
            />
        </template>
      </FormFragmentContainer>
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
    </div>
  </div>
</template>

<script lang="ts" setup>
import WrappedSelect from 'components/global/WrappedSelect.vue';
import WrappedTextInput from 'components/global/WrappedTextInput.vue';
import WrappedToggle from 'components/global/WrappedToggle.vue';
import { useGlobalStore } from 'stores/global';
import { storeToRefs } from 'pinia';
import FormFragmentContainer from '../global/FormFragmentContainer.vue';

const globalStore = useGlobalStore();
globalStore.fetch();
const {
  ratioOptions,
  shadersetOptions,
  shadersetfileOptions,
  global,
} = storeToRefs(globalStore);
</script>
