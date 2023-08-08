<!--
@author Nicolas TESSIER aka Asthonishia
-->
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
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.display.ratio.help') }}
            </template>
          </WrappedSelect>
          <WrappedSelect
            label="emulation.global.display.videomode.label"
            :options="videomodeOptions"
            :getter="global.videomode"
            :setter="globalStore.post"
            apiKey="videomode"
            v-if="global.videomode"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.display.videomode.help') }}
            </template>
          </WrappedSelect>
          <q-separator/>
          <WrappedToggle
            label="emulation.global.display.shaders.label"
            :getter="global.shaders"
            :setter="globalStore.post"
            apiKey="shaders"
            v-if="global.shaders"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.display.shaders.help') }}
            </template>
          </WrappedToggle>
          <WrappedSelect
            label="emulation.global.display.shaderset.label"
            :options="shadersetOptions"
            :getter="global.shaderset"
            :setter="globalStore.post"
            apiKey="shaderset"
            v-if="global.shaderset"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.display.shaderset.help.availableShaders') }}
              <ul>
                <li v-html="$t('emulation.global.display.shaderset.help.custom')"></li>
                <li v-html="$t('emulation.global.display.shaderset.help.none')"></li>
                <li v-html="$t('emulation.global.display.shaderset.help.retro')"></li>
                <li v-html="$t('emulation.global.display.shaderset.help.scanlines')"></li>
              </ul>
            </template>
          </WrappedSelect>
          <WrappedSelect
            label="emulation.global.display.customshaders.label"
            :options="shadersetfileOptions"
            :getter="global['shaderset.file']"
            :setter="globalStore.post"
            apiKey="shaderset.file"
            v-if="global['shaderset.file']"
            :disable="global.shaderset.value !== 'custom'"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.display.customshaders.help') }}
            </template>
          </WrappedSelect>
          <q-separator/>
          <WrappedToggle
            label="emulation.global.display.integerscale.label"
            :getter="global.integerscale"
            :setter="globalStore.post"
            apiKey="integerscale"
            v-if="global.integerscale"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.display.integerscale.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.global.display.smooth.label"
            :getter="global.smooth"
            :setter="globalStore.post"
            apiKey="smooth"
            v-if="global.smooth"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.display.smooth.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.global.display.recalboxoverlays.label"
            :getter="global.recalboxoverlays"
            :setter="globalStore.post"
            apiKey="showfps"
            v-if="global.recalboxoverlays"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.display.recalboxoverlays.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.global.display.showfps.label"
            :getter="global.showfps"
            :setter="globalStore.post"
            apiKey="showfps"
            v-if="global.showfps"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.display.showfps.help') }}
            </template>
          </WrappedToggle>

        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.global.saves.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.global.saves.autosave.label"
            :getter="global.autosave"
            :setter="globalStore.post"
            apiKey="autosave"
            v-if="global.autosave"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.saves.autosave.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.global.saves.showsavestatebeforerun.label"
            :getter="global['show.savestate.before.run']"
            :setter="globalStore.post"
            apiKey="show.savestate.before.run"
            v-if="global['show.savestate.before.run']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.saves.showsavestatebeforerun.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.global.security.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.global.security.quitpresstwice.label"
            :getter="global.quitpresstwice"
            :setter="globalStore.post"
            apiKey="quitpresstwice"
            v-if="global.quitpresstwice"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.security.quitpresstwice.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="emulation.global.virtualarcade.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.global.virtualarcade.activate.label"
            :getter="emulationstation.virtualarcade"
            :setter="emulationstationStore.post"
            apiKey="virtualarcade"
            v-if="emulationstation.virtualarcade"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.virtualarcade.activate.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.global.virtualarcade.includeNeogeo.label"
            :getter="emulationstation['virtualarcade.includeneogeo']"
            :setter="emulationstationStore.post"
            apiKey="virtualarcade.includeneogeo"
            v-if="emulationstation['virtualarcade.includeneogeo']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.virtualarcade.includeNeogeo.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.global.virtualarcade.hideOriginals.label"
            :getter="emulationstation['virtualarcade.hideoriginals']"
            :setter="emulationstationStore.post"
            apiKey="virtualarcade.hideoriginals"
            v-if="emulationstation['virtualarcade.hideoriginals']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.virtualarcade.hideOriginals.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.global.arcade.view.enhanced.label"
            :getter="emulationstation['arcade.view.enhanced']"
            :setter="emulationstationStore.post"
            apiKey="arcade.view.enhanced"
            v-if="emulationstation['arcade.view.enhanced']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.arcade.view.enhanced.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.global.arcade.view.hideclones.label"
            :getter="emulationstation['arcade.view.hideclones']"
            :setter="emulationstationStore.post"
            apiKey="arcade.view.hideclones"
            v-if="emulationstation['arcade.view.hideclones']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.arcade.view.hideclones.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.global.arcade.view.hidebios.label"
            :getter="emulationstation['arcade.view.hidebios']"
            :setter="emulationstationStore.post"
            apiKey="arcade.view.hidebios"
            v-if="emulationstation['arcade.view.hidebios']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.arcade.view.hidebios.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.global.arcade.view.hidenonworking.label"
            :getter="emulationstation['arcade.view.hidenonworking']"
            :setter="emulationstationStore.post"
            apiKey="arcade.view.hidenonworking"
            v-if="emulationstation['arcade.view.hidenonworking']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.arcade.view.hidenonworking.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="emulation.global.arcade.view.usedatabasenames.label"
            :getter="emulationstation['arcade.view.usedatabasenames']"
            :setter="emulationstationStore.post"
            apiKey="arcade.view.usedatabasenames"
            v-if="emulationstation['arcade.view.usedatabasenames']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.arcade.view.usedatabasenames.help') }}
            </template>
          </WrappedToggle>
          <WrappedMultipleSelect
            label="emulation.global.virtualarcade.manufacturers.label"
            :options="virtualArcadeManufacturersOptions"
            :getter="emulationstation['virtualarcade.manufacturers']"
            :setter="emulationstationStore.post"
            apiKey="demo.systemlist"
            v-if="emulationstation['virtualarcade.manufacturers']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.virtualarcade.manufacturers.help') }}
            </template>
          </WrappedMultipleSelect>
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
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.translation.translate.help') }}
            </template>
          </WrappedToggle>

          <WrappedSelect
            label="emulation.global.translation.translateFrom.label"
            :options="translateFromOptions"
            :getter="global['translate.from']"
            :setter="globalStore.post"
            apiKey="translate.from"
            v-if="global['translate.from']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.translation.translateFrom.help') }}
            </template>
          </WrappedSelect>

          <WrappedSelect
            label="emulation.global.translation.translateTo.label"
            :options="translateToOptions"
            :getter="global['translate.to']"
            :setter="globalStore.post"
            apiKey="translate.to"
            v-if="global['translate.to']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.translation.translateTo.help') }}
            </template>
          </WrappedSelect>

          <WrappedTextInput
            label="emulation.global.translation.translateApiKey.label"
            :getter="global['translate.apikey']"
            :setter="globalStore.post"
            apiKey="translate.apikey"
            v-if="global['translate.apikey']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.translation.translateApiKey.help') }}
            </template>
          </WrappedTextInput>

          <WrappedTextInput
            label="emulation.global.translation.translateUrl.label"
            :getter="global['translate.url']"
            :setter="globalStore.post"
            apiKey="translate.url"
            v-if="global['translate.url']"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.translation.translateUrl.help') }}
            </template>
          </WrappedTextInput>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.global.rewind.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.global.rewind.label"
            warning
            :getter="global.rewind"
            :setter="globalStore.post"
            apiKey="rewind"
            v-if="global.rewind"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.rewind.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="emulation.global.softpatching.title">
        <template v-slot:content>
          <WrappedSelect
            label="emulation.global.softpatching.select.label"
            :options="softpatchingOptions"
            :getter="global.softpatching"
            :setter="globalStore.post"
            apiKey="softpatching"
            v-if="global.softpatching"
            help
          >
            <template v-slot:help>
              {{ $t('emulation.global.softpatching.select.help.availableOptions') }}
              <ul>
                <li v-html="$t('emulation.global.softpatching.select.help.0')"></li>
                <li v-html="$t('emulation.global.softpatching.select.help.1')"></li>
                <li v-html="$t('emulation.global.softpatching.select.help.2')"></li>
              </ul>
            </template>
          </WrappedSelect>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import WrappedSelect from 'components/ui-kit/WrappedSelect.vue';
import WrappedTextInput from 'components/ui-kit/WrappedTextInput.vue';
import WrappedToggle from 'components/ui-kit/WrappedToggle.vue';
import { useGlobalStore } from 'stores/configuration/global';
import { useEmulationstationStore } from 'stores/configuration/emulationstation';
import { storeToRefs } from 'pinia';
import FormFragmentContainer from 'components/ui-kit/FormFragmentContainer.vue';

const globalStore = useGlobalStore();
globalStore.fetch();
const {
  ratioOptions,
  shadersetOptions,
  shadersetfileOptions,
  translateFromOptions,
  translateToOptions,
  videomodeOptions,
  softpatchingOptions,
  global,
} = storeToRefs(globalStore);

const emulationstationStore = useEmulationstationStore();
emulationstationStore.fetch();
const {
  virtualArcadeManufacturersOptions,
  emulationstation,
} = storeToRefs(emulationstationStore);
</script>
