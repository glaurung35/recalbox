<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.emustation.options.title">
        <template v-slot:content>
          <WrappedSelect
            label="settings.emustation.options.menuStyle.label"
            :help="$t('settings.emustation.options.menuStyle.help')"
            :options="menuOptions"
            :getter="emulationstation.menu"
            :setter="emulationstationStore.post"
            apiKey="menu"
            v-if="emulationstation.menu"
          />

          <q-separator/>

          <WrappedSelect
            label="settings.emustation.options.systemSelectedAsDefault.label"
            :options="selectedsystemOptions"
            :getter="emulationstation.selectedsystem"
            :setter="emulationstationStore.post"
            apiKey="selectedsystem"
            v-if="emulationstation.selectedsystem"
          />

          <q-separator/>

          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.emustation.options.startFromFirstSystem"
              :help="$t('settings.emustation.options.help_2')"
              :getter="emulationstation.bootongamelist"
              :setter="emulationstationStore.post"
              apiKey="bootongamelist"
              v-if="emulationstation.bootongamelist"
            />
          </div>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.emustation.options.disableSystemSelection"
              :help="$t('settings.emustation.options.help_3')"
              :getter="emulationstation.hidesystemview"
              :setter="emulationstationStore.post"
              apiKey="hidesystemview"
              v-if="emulationstation.hidesystemview"
            />
          </div>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.emustation.options.showOnlyScrapedGames"
              :help="$t('settings.emustation.options.help_4')"
              :getter="emulationstation.gamelistonly"
              :setter="emulationstationStore.post"
              apiKey="gamelistonly"
              v-if="emulationstation.gamelistonly"
            />
          </div>
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.emustation.videoSnaps.title">
        <template v-slot:content>
          <WrappedTextInput
            label="settings.emustation.videoSnaps.delay.title"
            :help="$t('settings.emustation.videoSnaps.delay.help')"
            :getter="emulationstation['videosnaps.delay']"
            :setter="emulationstationStore.post"
            apiKey="videosnaps.delay"
            v-if="emulationstation['videosnaps.delay']"
          />

          <q-separator/>

          <WrappedTextInput
            label="settings.emustation.videoSnaps.loop.title"
            :help="$t('settings.emustation.videoSnaps.loop.help')"
            :getter="emulationstation['videosnaps.loop']"
            :setter="emulationstationStore.post"
            apiKey="videosnaps.loop"
            v-if="emulationstation['videosnaps.loop']"
          />
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import WrappedSelect from 'components/global/WrappedSelect.vue';
import WrappedToggle from 'components/global/WrappedToggle.vue';
import WrappedTextInput from 'components/global/WrappedTextInput.vue';
import { useEmulationstationStore } from 'stores/emulationstation';
import { storeToRefs } from 'pinia';
import FormFragmentContainer from '../global/FormFragmentContainer.vue';

const emulationstationStore = useEmulationstationStore();
emulationstationStore.fetch();
const { menuOptions, selectedsystemOptions, emulationstation } = storeToRefs(emulationstationStore);
</script>
