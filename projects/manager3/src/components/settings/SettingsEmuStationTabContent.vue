<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.emustation.options.title">
        <template v-slot:content>
          <WrappedSelect
            label="settings.emustation.options.menuStyle.label"
            :options="menuOptions"
            :getter="emulationstation.menu"
            :setter="emulationstationStore.post"
            apiKey="menu"
            v-if="emulationstation.menu"
          />
          <p class="help" v-html="$t('settings.emustation.options.help_1')"></p>

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

          <WrappedToggle
            label="settings.emustation.options.startFromFirstSystem"
            :getter="emulationstation.bootongamelist"
            :setter="emulationstationStore.post"
            apiKey="bootongamelist"
            v-if="emulationstation.bootongamelist"
          />
          <p class="help" v-html="$t('settings.emustation.options.help_2')"></p>

          <q-separator/>

          <WrappedToggle
            label="settings.emustation.options.disableSystemSelection"
            :getter="emulationstation.hidesystemview"
            :setter="emulationstationStore.post"
            apiKey="hidesystemview"
            v-if="emulationstation.hidesystemview"
          />
          <p class="help" v-html="$t('settings.emustation.options.help_3')"></p>

          <q-separator/>

          <WrappedToggle
            label="settings.emustation.options.showOnlyScrapedGames"
            :getter="emulationstation.gamelistonly"
            :setter="emulationstationStore.post"
            apiKey="gamelistonly"
            v-if="emulationstation.gamelistonly"
          />
          <p class="help" v-html="$t('settings.emustation.options.help_4')"></p>
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.emustation.videoSnaps.title">
        <template v-slot:content>
          <WrappedTextInput
            label="settings.emustation.videoSnaps.delay.title"
            :getter="emulationstation['videosnaps.delay']"
            :setter="emulationstationStore.post"
            apiKey="videosnaps.delay"
            v-if="emulationstation['videosnaps.delay']"
          />
          <p class="help" v-html="$t('settings.emustation.videoSnaps.delay.help')"></p>

          <q-separator/>

          <WrappedTextInput
            label="settings.emustation.videoSnaps.loop.title"
            :getter="emulationstation['videosnaps.loop']"
            :setter="emulationstationStore.post"
            apiKey="videosnaps.loop"
            v-if="emulationstation['videosnaps.loop']"
          />
          <p class="help" v-html="$t('settings.emustation.videoSnaps.loop.help')"></p>
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
