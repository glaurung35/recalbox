<!--
@author Pit64
-->
<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="token.title">
        <template v-slot:content>
          <WrappedTextInput
            label="token.patron.privatekey.label"
            :getter="patron.privatekey"
            :setter="patronStore.post"
            apiKey="privatekey"
            v-if="patron.privatekey"
            help
          >
            <template v-slot:help>
              {{ $t('token.patron.privatekey.help') }}
            </template>
          </WrappedTextInput>
          <WrappedSelect
            label="token.scraper.source.label"
            :options="sourceOptions"
            :getter="scraper.source"
            :setter="scraperStore.post"
            apiKey="source"
            v-if="scraper.source"
            help
          >
            <template v-slot:help>
              {{ $t('token.scraper.source.help') }}
            </template>
          </WrappedSelect>
          <WrappedToggle
            label="token.scraper.auto.label"
            :getter="scraper.auto"
            :setter="scraperStore.post"
            apiKey="auto"
            v-if="scraper.auto"
            help
          >
            <template v-slot:help>
              {{ $t('token.scraper.auto.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="token.music.remoteplaylist.enable.label"
            :getter="music['remoteplaylist.enable']"
            :setter="musicStore.post"
            apiKey="remoteplaylist.enable"
            v-if="music['remoteplaylist.enable']"
            help
          >
            <template v-slot:help>
              {{ $t('token.music.remoteplaylist.enable.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import FormFragmentContainer from 'components/ui-kit/FormFragmentContainer.vue';
import WrappedTextInput from 'components/ui-kit/WrappedTextInput.vue';
import WrappedSelect from 'components/ui-kit/WrappedSelect.vue';
import WrappedToggle from 'components/ui-kit/WrappedToggle.vue';
import { storeToRefs } from 'pinia';
import { usePatronStore } from 'stores/configuration/patron';
import { useMusicStore } from 'stores/configuration/music';
import { useScraperStore } from 'stores/configuration/scraper';

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
scraperStore.fetchOptions();
const {
  sourceOptions,
  scraper,
} = storeToRefs(scraperStore);
</script>
