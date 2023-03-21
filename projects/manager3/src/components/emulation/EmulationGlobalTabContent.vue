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
              :getter="global.rewind"
              :setter="globalStore.post"
              apiKey="rewind"
              v-if="global.rewind"
            />
          <p class="help">
            <q-icon class="text-red" name="mdi-alert" style="font-size: 2em"/>
            {{$t('emulation.global.rewind.warning')}}
          </p>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.global.autosave.title">
        <template v-slot:content>
            <WrappedToggle
              label="emulation.global.autosave.label"
              :getter="global.autosave"
              :setter="globalStore.post"
              apiKey="autosave"
              v-if="global.autosave"
            />
          <p class="help" v-html="$t('emulation.global.autosave.help')"></p>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.global.shaders.title">
        <template v-slot:content>
          <p class="help" v-html="$t('emulation.global.shaders.help')"></p>
            <WrappedSelect
              label="emulation.global.shaders.label"
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
              :getter="global.integerscale"
              :setter="globalStore.post"
              apiKey="integerscale"
              v-if="global.integerscale"
            />
          <p class="help" v-html="$t('emulation.global.integerscale.help')"></p>
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
          <p v-html="$t('emulation.global.retroachievements.help_1')"></p>
          <p v-html="$t('emulation.global.retroachievements.help_2')"></p>

          <WrappedToggle
            label="emulation.global.retroachievements.activate"
            :getter="global.retroachievements"
            :setter="globalStore.post"
            apiKey="retroachievements"
            v-if="global.retroachievements"
          />

          <q-separator/>

          <WrappedTextInput
            label="emulation.global.retroachievements.username"
            :getter="global['retroachievements.username']"
            :setter="globalStore.post"
            apiKey="retroachievements.username"
            v-if="global['retroachievements.username']"
          />
          <WrappedTextInput
            label="emulation.global.retroachievements.password"
            :getter="global['retroachievements.password']"
            :setter="globalStore.post"
            apiKey="retroachievements.password"
            v-if="global['retroachievements.password']"
            password
          />

          <q-separator/>

          <WrappedToggle
            label="emulation.global.retroachievements.hardcore"
            :getter="global['retroachievements.hardcore']"
            :setter="globalStore.post"
            apiKey="retroachievements.hardcore"
            v-if="global['retroachievements.hardcore']"
          />
          <p class="help">
            <q-icon class="text-red" name="mdi-alert" style="font-size: 2em"/>
            {{$t('emulation.global.retroachievements.help_3')}}
          </p>
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
