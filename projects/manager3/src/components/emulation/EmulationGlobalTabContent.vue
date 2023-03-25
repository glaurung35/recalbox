<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="emulation.global.display.title">
        <template v-slot:content>
          <WrappedSelect
            label="emulation.global.display.ratio.label"
            :options="ratioOptions"
            :getter="global.ratio"
            :setter="globalStore.post"
            apiKey="ratio"
            v-if="global.ratio"
          />
          <q-separator/>
          <WrappedSelect
            label="emulation.global.display.shaders.label"
            :help="$t('emulation.global.display.shaders.help')"
            :options="shadersetOptions"
            :getter="global.shaderset"
            :setter="globalStore.post"
            apiKey="shaderset"
            v-if="global.shaderset"
          />
          <WrappedSelect
            label="emulation.global.display.customshaders.label"
            :options="shadersetfileOptions"
            :getter="global['shaderset.file']"
            :setter="globalStore.post"
            apiKey="shaderset.file"
            v-if="global['shaderset.file']"
            :disable="global.shaderset.value !== 'custom'"
          />
          <q-separator/>
          <WrappedToggle
            label="emulation.global.display.integerscale.label"
            :help="$t('emulation.global.display.integerscale.help')"
            :getter="global.integerscale"
            :setter="globalStore.post"
            apiKey="integerscale"
            v-if="global.integerscale"
          />
          <q-separator/>
          <WrappedToggle
            label="emulation.global.display.smooth.label"
            :getter="global.smooth"
            :setter="globalStore.post"
            apiKey="smooth"
            v-if="global.smooth"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.global.rewind.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.global.rewind.label"
            :help="$t('emulation.global.rewind.help')"
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
            label="emulation.global.netplay.systems.label"
            :help="$t('emulation.global.netplay.systems.help')"
            :getter="global['netplay.systems']"
            :setter="globalStore.post"
            apiKey="netplay.systems"
            v-if="global['netplay.systems']"
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
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
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
      <FormFragmentContainer title="emulation.global.arcade.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.global.arcade.activate.label"
            :help="$t('emulation.global.arcade.activate.help')"
            :getter="global.arcade"
            :setter="globalStore.post"
            apiKey="arcade"
            v-if="global.arcade"
          />
          <WrappedTextInput
            label="emulation.global.arcade.position.label"
            :help="$t('emulation.global.arcade.position.help')"
            :getter="global['arcade.position']"
            :setter="globalStore.post"
            apiKey="arcade.position"
            v-if="global['arcade.position']"
          />
          <WrappedToggle
            label="emulation.global.arcade.includeNeogeo.label"
            :help="$t('emulation.global.arcade.includeNeogeo.help')"
            :getter="global['arcade.includeneogeo']"
            :setter="globalStore.post"
            apiKey="arcade.includeneogeo"
            v-if="global['arcade.includeneogeo']"
          />
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="emulation.global.arcade.hideOriginals.label"
              :help="$t('emulation.global.arcade.hideOriginals.help')"
              :getter="global['arcade.hideoriginals']"
              :setter="globalStore.post"
              apiKey="arcade.hideoriginals"
              v-if="global['arcade.hideoriginals']"
            />
          </div>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.global.translation.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.global.translation.translate.label"
            :getter="global.translate"
            :setter="globalStore.post"
            apiKey="translate"
            v-if="global.translate"
          />

          <WrappedSelect
            label="emulation.global.translation.translateFrom.label"
            :help="$t('emulation.global.translation.translateFrom.help')"
            :options="translateFromOptions"
            :getter="global['translate.from']"
            :setter="globalStore.post"
            apiKey="translate.from"
            v-if="global['translate.from']"
          />

          <WrappedSelect
            label="emulation.global.translation.translateTo.label"
            :help="$t('emulation.global.translation.translateTo.help')"
            :options="translateToOptions"
            :getter="global['translate.to']"
            :setter="globalStore.post"
            apiKey="translate.to"
            v-if="global['translate.to']"
          />

          <WrappedTextInput
            label="emulation.global.translation.translateApiKey.label"
            :help="$t('emulation.global.translation.translateApiKey.help')"
            :getter="global['translate.apikey']"
            :setter="globalStore.post"
            apiKey="translate.apikey"
            v-if="global['translate.apikey']"
          />

          <WrappedTextInput
            label="emulation.global.translation.translateUrl.label"
            :help="$t('emulation.global.translation.translateUrl.help')"
            :getter="global['translate.url']"
            :setter="globalStore.post"
            apiKey="translate.url"
            v-if="global['translate.url']"
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
  translateFromOptions,
  translateToOptions,
  global,
} = storeToRefs(globalStore);
</script>
