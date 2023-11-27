<template>
  <div class="header">
    <div class="title">
      <q-icon name="mdi-twitch" size="md" @click="openURL(twitchUrl)">
        <q-tooltip
          anchor="center right"
          self="center left"
          :offset="[10, 10]"
          class="bg-primary"
          content-style="font-size: 16px"
        >
          {{ $t('home.twitch.our_channel') }}
        </q-tooltip>
      </q-icon>
      {{ $t('home.twitch.title') }}
    </div>
  </div>
  <div class="events">
    <ListSkeletonLoader :iterations="7" :loader="loadingSchedule"/>
    <transition
      appear
      enter-active-class="animated flipInX"
    >
      <q-list v-if="schedule && !loadingSchedule">
        <q-item
          v-for="segment in schedule"
          :key="segment.id"
          clickable
          @click="openURL(twitchUrl + '/schedule')"
        >
          <q-tooltip
            anchor="center right"
            self="center left"
            :offset="[10, 10]"
            class="bg-primary"
            content-style="font-size: 16px"
            v-if="segment.is_recurring"
          >
            {{ $t('home.twitch.is_recurring') }}
          </q-tooltip>
          <q-item-section avatar>
            <q-avatar
              color="light-blue"
              text-color="white"
              :icon="segment.is_recurring ? 'mdi-calendar-sync-outline' : 'mdi-calendar-month'"
              square
            />
          </q-item-section>
          <q-item-section>
            <q-item-label class="title">{{segment.title}}</q-item-label>
            <q-item-label class="date" caption>{{ formatDateTime(segment.start_time) }}</q-item-label>
          </q-item-section>
        </q-item>
      </q-list>
    </transition>
  </div>
</template>

<script lang="ts" setup>
import { date, openURL } from 'quasar';
import { useTwitchStore } from 'stores/twitch';
import { storeToRefs } from 'pinia';
import ListSkeletonLoader from 'components/ui-kit/ListSkeletonLoader.vue';

const twitchStore = useTwitchStore();
const { schedule, loadingSchedule } = storeToRefs(twitchStore);
const twitchUrl = process.env.TWITCH_URL;

twitchStore.getSchedule();

function formatDateTime(timestamp: string) {
  return date.formatDate(timestamp, 'DD/MM/YYYY - HH:mm');
}
</script>

<style lang="sass" scoped>
.header
  display: flex
  justify-content: space-between
  gap: 1em
  padding: 0.2em
  border-bottom: 2px solid white
  border-top: 2px solid $rc-light-grey

  .title
    display: flex
    align-items: center
    text-transform: uppercase
    font-weight: 400
    min-height: 34px
    margin-left: 1em

    .q-icon
      cursor: pointer
      margin-right: 0.2em

      &:hover
        color: $twitch

.events
  color: rgba(0, 0, 0, 0.54)

  .date
    color: $light-blue

  //.q-list--separator > .q-item-type + .q-item-type
  //  border-top: 2px solid $rc-light-grey
</style>
