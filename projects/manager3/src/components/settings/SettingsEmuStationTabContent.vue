<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="row">
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
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.menus.systemSelectedAsDefault.help') }}
            </template>
          </WrappedSelect>

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
            label="settings.emustation.menus.quickSystemSelect.label"
            :getter="emulationstation.quicksystemselect"
            :setter="emulationstationStore.post"
            apiKey="quicksystemselect"
            v-if="emulationstation.quicksystemselect"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.menus.quickSystemSelect.help') }}
            </template>
          </WrappedToggle>
          <WrappedSelect
            label="settings.emustation.menus.systemsorting.label"
            :options="systemsortingOptions"
            :getter="emulationstation.systemsorting"
            :setter="emulationstationStore.post"
            apiKey="systemsorting"
            v-if="emulationstation.systemsorting"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.menus.systemsorting.help.availableOptions') }}
              <ul>
                <li v-html="$t('settings.emustation.menus.systemsorting.help.0')"></li>
                <li v-html="$t('settings.emustation.menus.systemsorting.help.1')"></li>
                <li v-html="$t('settings.emustation.menus.systemsorting.help.2')"></li>
                <li v-html="$t('settings.emustation.menus.systemsorting.help.3')"></li>
                <li v-html="$t('settings.emustation.menus.systemsorting.help.4')"></li>
                <li v-html="$t('settings.emustation.menus.systemsorting.help.5')"></li>
                <li v-html="$t('settings.emustation.menus.systemsorting.help.6')"></li>
                <li v-html="$t('settings.emustation.menus.systemsorting.help.7')"></li>
                <li v-html="$t('settings.emustation.menus.systemsorting.help.8')"></li>
              </ul>
            </template>
          </WrappedSelect>
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
            label="settings.emustation.menus.clock.label"
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

      <FormFragmentContainer title="settings.emustation.filters.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.emustation.filters.showonlylatestversion.label"
            :getter="emulationstation.showonlylatestversion"
            :setter="emulationstationStore.post"
            apiKey="showonlylatestversion"
            v-if="emulationstation.showonlylatestversion"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.filters.showonlylatestversion.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.filters.favoritesonly.label"
            :getter="emulationstation.favoritesonly"
            :setter="emulationstationStore.post"
            apiKey="favoritesonly"
            v-if="emulationstation.favoritesonly"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.filters.favoritesonly.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.filters.showhidden.label"
            :getter="emulationstation.showhidden"
            :setter="emulationstationStore.post"
            apiKey="showhidden"
            v-if="emulationstation.showhidden"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.filters.showhidden.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.filters.filteradultgames.label"
            :getter="emulationstation.filteradultgames"
            :setter="emulationstationStore.post"
            apiKey="filteradultgames"
            v-if="emulationstation.filteradultgames"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.filters.filteradultgames.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.filters.hidepreinstalledgames.label"
            :getter="global.hidepreinstalledgames"
            :setter="globalStore.post"
            apiKey="hidepreinstalledgames"
            v-if="global.hidepreinstalledgames"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.filters.hidepreinstalledgames.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.filters.hidenogames.label"
            :getter="emulationstation.hidenogames"
            :setter="emulationstationStore.post"
            apiKey="hidenogames"
            v-if="emulationstation.hidenogames"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.filters.hidenogames.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.emustation.videoSnaps.title">
        <template v-slot:content>
          <WrappedSlider
            label="settings.emustation.videoSnaps.delay.label"
            :getter="emulationstation['videosnaps.delay']"
            :setter="emulationstationStore.post"
            apiKey="videosnaps.delay"
            v-if="emulationstation['videosnaps.delay']"
            :min="videosnapsDelayOptions.lowerValue"
            :step="500"
            :max="videosnapsDelayOptions.higherValue"
            icon="mdi-clock-start"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.videoSnaps.delay.help') }}
            </template>
          </WrappedSlider>
          <WrappedSlider
            label="settings.emustation.videoSnaps.loop.label"
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
              {{ $t('settings.emustation.videoSnaps.loop.help.availableOptions') }}
              <ul>
                <li v-html="$t('settings.emustation.videoSnaps.loop.help.0')"></li>
                <li v-html="$t('settings.emustation.videoSnaps.loop.help.1')"></li>
              </ul>
            </template>
          </WrappedSlider>
        </template>
      </FormFragmentContainer>

      <FormFragmentContainer title="settings.emustation.virtualsystems.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.emustation.virtualsystems.allgames.label"
            :getter="emulationstation['collection.allgames']"
            :setter="emulationstationStore.post"
            apiKey="collection.allgames"
            v-if="emulationstation['collection.allgames']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.virtualsystems.allgames.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.virtualsystems.multiplayer.label"
            :getter="emulationstation['collection.multiplayer']"
            :setter="emulationstationStore.post"
            apiKey="collection.multiplayer"
            v-if="emulationstation['collection.multiplayer']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.virtualsystems.multiplayer.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.virtualsystems.lastplayed.label"
            :getter="emulationstation['collection.lastplayed']"
            :setter="emulationstationStore.post"
            apiKey="collection.lastplayed"
            v-if="emulationstation['collection.lastplayed']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.virtualsystems.lastplayed.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.virtualsystems.lightgun.label"
            :getter="emulationstation['collection.lightgun.hide']"
            :setter="emulationstationStore.post"
            apiKey="collection.lightgun.hide"
            v-if="emulationstation['collection.lightgun.hide']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.virtualsystems.lightgun.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.virtualsystems.ports.label"
            :getter="emulationstation['collection.ports.hide']"
            :setter="emulationstationStore.post"
            apiKey="collection.ports.hide"
            v-if="emulationstation['collection.ports.hide']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.virtualsystems.ports.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.emustation.virtualsystems.tate.label"
            :getter="emulationstation['collection.tate']"
            :setter="emulationstationStore.post"
            apiKey="collection.tate"
            v-if="emulationstation['collection.tate']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.virtualsystems.tate.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.emustation.theme.title">
        <template v-slot:content>
          <WrappedSelect
            label="settings.emustation.theme.folder.label"
            :options="themeFolderOptions"
            :getter="emulationstation['theme.folder']"
            :setter="emulationstationStore.post"
            apiKey="theme.folder"
            v-if="emulationstation['theme.folder']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.theme.folder.help') }}
            </template>
          </WrappedSelect>
          <WrappedSelect
            label="settings.emustation.theme.transition.label"
            :options="themeTransitionOptions"
            :getter="emulationstation['theme.transition']"
            :setter="emulationstationStore.post"
            apiKey="theme.transition"
            v-if="emulationstation['theme.transition']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.theme.transition.help.availableOptions') }}
              <ul>
                <li v-html="$t('settings.emustation.theme.transition.help.0')"></li>
                <li v-html="$t('settings.emustation.theme.transition.help.1')"></li>
                <li v-html="$t('settings.emustation.theme.transition.help.2')"></li>
              </ul>
            </template>
          </WrappedSelect>
          <WrappedToggle
            label="settings.emustation.theme.carousel.label"
            :getter="emulationstation['theme.carousel']"
            :setter="emulationstationStore.post"
            apiKey="theme.carousel"
            v-if="emulationstation['theme.carousel']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.theme.carousel.help.availableOptions') }}
              <ul>
                <li v-html="$t('settings.emustation.theme.carousel.help.0')"></li>
                <li v-html="$t('settings.emustation.theme.carousel.help.1')"></li>
              </ul>
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.emustation.popups.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.emustation.popups.showhelp.label"
            :getter="emulationstation.showhelp"
            :setter="emulationstationStore.post"
            apiKey="showhelp"
            v-if="emulationstation.showhelp"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.popups.showhelp.help') }}
            </template>
          </WrappedToggle>
          <WrappedSlider
            label="settings.emustation.popups.popup.help.label"
            :getter="emulationstation['popoup.help']"
            :setter="emulationstationStore.post"
            apiKey="popoup.help"
            v-if="emulationstation['popoup.help']"
            :min="popupHelpOptions.lowerValue"
            :max="popupHelpOptions.higherValue"
            icon="mdi-comment-processing-outline"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.popups.popup.help.help') }}
            </template>
          </WrappedSlider>
          <WrappedSlider
            label="settings.emustation.popups.popup.music.label"
            :getter="emulationstation['popoup.music']"
            :setter="emulationstationStore.post"
            apiKey="popoup.music"
            v-if="emulationstation['popoup.music']"
            :min="popupMusicOptions.lowerValue"
            :max="popupMusicOptions.higherValue"
            icon="mdi-comment-processing-outline"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.popups.popup.music.help') }}
            </template>
          </WrappedSlider>
          <WrappedSlider
            label="settings.emustation.popups.popup.netplay.label"
            :getter="emulationstation['popoup.netplay']"
            :setter="emulationstationStore.post"
            apiKey="popoup.netplay"
            v-if="emulationstation['popoup.netplay']"
            :min="popupNetplayOptions.lowerValue"
            :max="popupNetplayOptions.higherValue"
            icon="mdi-comment-processing-outline"
            help
          >
            <template v-slot:help>
              {{ $t('settings.emustation.popups.popup.netplay.help') }}
            </template>
          </WrappedSlider>
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
import { useGlobalStore } from 'stores/configuration/global';

const emulationstationStore = useEmulationstationStore();
emulationstationStore.fetch();

const systemStore = useSystemStore();
systemStore.fetch();

const {
  menuOptions,
  selectedsystemOptions,
  videosnapsLoopOptions,
  videosnapsDelayOptions,
  themeFolderOptions,
  themeTransitionOptions,
  popupHelpOptions,
  popupMusicOptions,
  popupNetplayOptions,
  systemsortingOptions,
  emulationstation,
} = storeToRefs(emulationstationStore);
const { esVideomodeOptions, system } = storeToRefs(systemStore);

const globalStore = useGlobalStore();
globalStore.fetch();
const {
  global,
} = storeToRefs(globalStore);
</script>
