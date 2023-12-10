<template>
  <div class="header">
    <div class="title">
      <q-icon name="icon-emustation" size="md"/>
      {{ $t('home.game.title') }}
    </div>
    <div class="controls">
      <q-btn
        v-if="currentState.currentRom?.metaData"
        flat
        rounded
        square
        icon="mdi-information"
        @click="() => infoOpen = true"
      >
        <q-tooltip
          class="bg-primary"
          :offset="[10, 10]"
          content-class="bg-primary"
          content-style="font-size: 16px"
        >
          {{ $t('home.game.metaData.iconLabel') }}
        </q-tooltip>
      </q-btn>
      <q-btn
        v-if="currentState.currentRom"
        flat
        rounded
        square
        icon="mdi-stop"
        text-color="negative"
        disable
      >
        <q-tooltip
          class="bg-primary"
          :offset="[10, 10]"
          content-class="bg-primary"
          content-style="font-size: 16px"
        >
          {{ $t('home.game.stop') }}
        </q-tooltip>
      </q-btn>
    </div>
  </div>
  <div class="informations">
    <OverlayMessage
      background
      icon="mdi-sleep"
      v-if="currentState.currentAction === Actions.sleep"
    />
    <OverlayMessage
      icon="mdi-ghost-off-outline"
      v-else-if="currentState.currentRom === null"
    />
    <div class="screen">
      <q-img
        v-if="currentState.currentRom"
        :src="currentState.currentRom?.imagePath"
        spinner-color="white"
        fit="contain"
      />
    </div>
  </div>
  <q-dialog
    v-if="currentState.currentRom?.metaData"
    transition-hide="slide-down"
    transition-show="slide-up"
    v-model="infoOpen"
  >
    <q-card class="bg-primary text-white info-dialog-card background info-dialog">
      <q-card-section class="text-justify" style="white-space: pre-line;">
        <div class="text-uppercase text-bold">{{ currentState.currentRom?.metaData?.name }}</div>
        <hr />
        <div>{{ currentState.currentRom?.metaData?.synopsys }}</div>
        <hr />
        <table>
          <tr
            v-if="currentState.currentRom?.metaData?.publisher"
          >
            <td>{{ $t('home.game.metaData.modal.publisher') }}</td>
            <td class="value">{{ currentState.currentRom?.metaData?.publisher }}</td>
          </tr>
          <tr
            v-if="currentState.currentRom?.metaData?.developer"
          >
            <td>{{ $t('home.game.metaData.modal.developer') }}</td>
            <td class="value">{{ currentState.currentRom?.metaData?.developer }}</td>
          </tr>
          <tr
            v-if="currentState.currentRom?.metaData?.releaseDate"
          >
            <td>{{ $t('home.game.metaData.modal.releaseDate') }}</td>
            <td class="value">{{ new Date(currentState.currentRom?.metaData?.releaseDate * 1000).toLocaleDateString($i18n.locale) }}</td>
          </tr>
          <tr
            v-if="currentState.currentRom?.metaData?.rating"
          >
            <td>{{ $t('home.game.metaData.modal.rating') }}</td>
            <td class="value">
              <q-rating
                :model-value="currentState.currentRom?.metaData?.rating * 5"
                class=“no-shadow”
                color="accent"
                readonly
                size="1em"
                icon="stars"
              />
            </td>
          </tr>
          <tr>
            <td>{{ $t('home.game.metaData.modal.players') }}</td>
            <td class="value">
              {{ currentState.currentRom?.metaData?.players.min }}
              - {{ currentState.currentRom?.metaData?.players.max }}
            </td>
          </tr>
          <tr>
            <td>{{ $t('home.game.metaData.modal.genres') }}</td>
            <td class="value">{{ currentState.currentRom?.metaData?.genres.free }}</td>
          </tr>
        </table>
      </q-card-section>
    </q-card>
  </q-dialog>
</template>

<script lang="ts" setup>
import OverlayMessage from 'components/ui-kit/OverlayMessage.vue';
import { useEmulationstationStore } from 'stores/configuration/emulationstation';
import { Actions } from 'stores/types/mqtt';
import { ref } from 'vue';

const emulationStationStore = useEmulationstationStore();
const { currentState } = emulationStationStore;

const infoOpen = ref<boolean>(false);
</script>

<style lang="sass" scoped>
.value
  padding-left: 2em

.header
  display: flex
  justify-content: space-between
  gap: 1em
  margin-bottom: 1em
  padding: 0.2em
  border-bottom: 2px solid white

  .title
    display: flex
    align-items: center
    text-transform: uppercase
    font-weight: 400
    color: $accent
    min-height: 36px
    margin-left: 1em

    .q-icon
      margin-right: 0.2em

.informations
  position: relative
  display: flex
  justify-content: center
  padding: 0 1em
  height: 100%
  align-items: center

  .screen
    position: relative
    width: 50%

.info-dialog-card
  border-left: 6px solid $accent

  &:before
    content: "\F02FC"

@keyframes helpDialogSlidein
  from
    right: -10rem
    bottom: -10rem

  to
    right: -30px
    bottom: 0

.info-dialog
  overflow: hidden!important
  &:before
    font-size: 14em
    bottom: 0
    right: -30px
    color: $secondary
    opacity: 0.3
    animation-duration: .5s
    animation-name: helpDialogSlidein
</style>
