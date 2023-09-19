<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-page-sticky
    position="bottom"
    style="transform: translate(36px, 0px);"
  >
    <q-fab
      color="primary"
      direction="up"
      flat
      icon="mdi-web"
      id="help-button"
      square
      vertical-actions-align="right"
      @update:model-value="update"
      :model-value="model"
    >
      <q-fab-action
        @click="() => openURL(urls.patreon)"
        color="secondary"
        icon="mdi-patreon"
        label="Patreon"
        label-position="left"
        square
      />
      <q-fab-action
        @click="() => openURL(urls.gitlab)"
        color="secondary"
        icon="mdi-gitlab"
        label="Gitlab"
        label-position="left"
        square
      />
      <q-fab-action
        @click="() => openURL(urls.forum)"
        color="secondary"
        icon="mdi-forum"
        label="Forum"
        label-position="left"
        square
      />
      <q-fab-action
        @click="() => openURL(urls.wiki)"
        class="wiki"
        color="secondary"
        icon="icon-gitbook"
        label="Documentation"
        label-position="left"
        square
      />
      <q-fab-action
        @click="() => openURL(urls.discord)"
        color="secondary"
        icon="mdi-discord"
        label="Discord"
        label-position="left"
        square
      />
    </q-fab>
  </q-page-sticky>
</template>

<script lang="ts" setup>
import { openURL } from 'quasar';
import { toRefs } from 'vue';

const urls: object = {
  discord: process.env.DISCORD_URL,
  wiki: process.env.WIKI_URL,
  forum: process.env.FORUM_URL,
  gitlab: process.env.GITLAB_URL,
  patreon: process.env.PATREON_URL,
};
const emit = defineEmits(['toggle']);

const props = defineProps({
  model: { type: Boolean, required: true },
});

const { model } = toRefs(props);

function update(value:boolean) {
  emit('toggle', { label: 'shortcutsButton', value });
}

</script>

<style lang="sass">
.q-page-container
  #help-button
    .q-btn--fab-mini:hover
      .q-icon:before
        color: $accent

    .wiki
      i
        font-size: 18px
        margin-left: 3px
        margin-right: 3px
</style>
