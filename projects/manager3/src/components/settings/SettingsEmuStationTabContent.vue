<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.emustation.display.title">
        <template v-slot:content>
          <WrappedSelect
            label="settings.emustation.display.videomode.label"
            :options="esVideomodeOptions"
            :getter="system['es.videomode']"
            :setter="systemStore.post"
            apiKey="es.videomode"
            v-if="system['es.videomode']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.display.videomode.help') }}
            </template>
          </WrappedSelect>
        </template>
      </FormFragmentContainer>
    </div>
      <FormFragmentContainer title="settings.emustation.menus.title">
        <template v-slot:content>
          <WrappedSelect
            label="settings.emustation.menus.menuStyle.label"
            :options="menuOptions"
            :getter="emulationstation.menu"
            :setter="emulationstationStore.post"
            apiKey="menu"
            v-if="emulationstation.menu"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.menus.menuStyle.help.availableOptions') }}
              <ul>
                <li v-html="$t('settings.emustation.menus.menuStyle.help.bartop')"></li>
                <li v-html="$t('settings.emustation.menus.menuStyle.help.default')"></li>
                <li v-html="$t('settings.emustation.menus.menuStyle.help.none')"></li>
              </ul>
            </template>
          </WrappedSelect>

          <q-separator/>

          <WrappedSelect
            label="settings.emustation.menus.systemSelectedAsDefault.label"
            :options="selectedsystemOptions"
            :getter="emulationstation.selectedsystem"
            :setter="emulationstationStore.post"
            apiKey="selectedsystem"
            v-if="emulationstation.selectedsystem"
          />

          <q-separator/>

          <WrappedToggle
            label="settings.emustation.menus.startFromFirstSystem.label"
            :getter="emulationstation.bootongamelist"
            :setter="emulationstationStore.post"
            apiKey="bootongamelist"
            v-if="emulationstation.bootongamelist"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.menus.startFromFirstSystem.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.menus.disableSystemSelection.label"
            :getter="emulationstation.hidesystemview"
            :setter="emulationstationStore.post"
            apiKey="hidesystemview"
            v-if="emulationstation.hidesystemview"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.menus.disableSystemSelection.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.menus.showOnlyScrapedGames.label"
            :getter="emulationstation.gamelistonly"
            :setter="emulationstationStore.post"
            apiKey="gamelistonly"
            v-if="emulationstation.gamelistonly"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.menus.showOnlyScrapedGames.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.menus.clock.title"
            :getter="emulationstation.clock"
            :setter="emulationstationStore.post"
            apiKey="clock"
            v-if="emulationstation.clock"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.menus.clock.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.emustation.videoSnaps.title">
        <template v-slot:content>
          <WrappedSlider
            label="settings.emustation.videoSnaps.delay.title"
            :getter="emulationstation['videosnaps.delay']"
            :setter="emulationstationStore.post"
            apiKey="videosnaps.delay"
            v-if="emulationstation['videosnaps.delay']"
            :min="videosnapsDelayOptions.lowerValue"
            :max="videosnapsDelayOptions.higherValue"
            icon="mdi-clock-start"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.videoSnaps.delay.help') }}
            </template>
          </WrappedSlider>
          <WrappedSlider
            label="settings.emustation.videoSnaps.loop.title"
            :getter="emulationstation['videosnaps.loop']"
            :setter="emulationstationStore.post"
            apiKey="videosnaps.loop"
            v-if="emulationstation['videosnaps.loop']"
            :min="videosnapsLoopOptions.lowerValue"
            :max="videosnapsLoopOptions.higherValue"
            icon="mdi-reload"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.videoSnaps.loop.help') }}
            </template>
          </WrappedSlider>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.patreon.title">
        <template v-slot:content>
          <WrappedTextInput
            label="settings.patreon.patron.privatekey.title"
            :getter="patron.privatekey"
            :setter="patronStore.post"
            apiKey="privatekey"
            v-if="patron.privatekey"
            help
          >
            <template v-slot:help>
              {{ $t('settings.patreon.patron.privatekey.help') }}
            </template>
          </WrappedTextInput>
          <WrappedToggle
            label="settings.music.remoteplaylist.enable.title"
            :getter="music['remoteplaylist.enable']"
            :setter="musicStore.post"
            apiKey="remoteplaylist.enable"
            v-if="music['remoteplaylist.enable']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.music.remoteplaylist.enable.help') }}
            </template>
          </WrappedToggle>
          <WrappedSelect
            label="settings.scraper.settings.source.title"
            :options="sourceOptions"
            :getter="scraper.source"
            :setter="scraperStore.post"
            apiKey="source"
            v-if="scraper.source"
            help
          >
            <template v-slot:help>
              {{ $t('settings.scraper.settings.source.help') }}
            </template>
        </WrappedSelect>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import WrappedSelect from 'components/ui-kit/WrappedSelect.vue';
import WrappedToggle from 'components/ui-kit/WrappedToggle.vue';
import { useEmulationstationStore } from 'stores/configuration/emulationstation';
import { storeToRefs } from 'pinia';
import { useSystemStore } from 'stores/configuration/system';
import WrappedSlider from 'components/ui-kit/WrappedSlider.vue';
import FormFragmentContainer from 'components/ui-kit/FormFragmentContainer.vue';
import { usePatronStore } from 'stores/configuration/patron';
import { useMusicStore } from 'stores/configuration/music';
import { useScraperStore } from 'stores/configuration/scraper';
import WrappedTextInput from 'components/ui-kit/WrappedTextInput.vue';

const emulationstationStore = useEmulationstationStore();
emulationstationStore.fetch();

const systemStore = useSystemStore();
systemStore.fetch();

const {
  menuOptions,
  selectedsystemOptions,
  videosnapsLoopOptions,
  videosnapsDelayOptions,
  emulationstation,
} = storeToRefs(emulationstationStore);
const { esVideomodeOptions, system } = storeToRefs(systemStore);

const patronStore = usePatronStore();
patronStore.fetch();
const {
  patron,
} = storeToRefs(patronStore);

const musicStore = useMusicStore();
musicStore.fetch();
const {
  music,
} = storeToRefs(musicStore);

const scraperStore = useScraperStore();
scraperStore.fetch();
const {
  sourceOptions,
  scraper,
} = storeToRefs(scraperStore);
</script>
