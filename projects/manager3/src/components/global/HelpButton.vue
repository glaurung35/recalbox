<template>
  <q-page-sticky :offset="[18, 18]" position="bottom-right">
    <q-fab
      color="primary"
      direction="up"
      flat
      icon="mdi-help-circle-outline"
      id="help-button"
      square
      vertical-actions-align="right"
    >
      <q-fab-action
        color="secondary"
        icon="mdi-bash"
        label="recalbox-support.sh"
        label-position="left"
        square
        target="_blank"
      />
      <q-fab-action
        :href="urls.forum"
        color="secondary"
        icon="mdi-forum"
        label="Forum"
        label-position="left"
        square
        target="_blank"
      />
      <q-fab-action
        :href="urls.discord"
        color="secondary"
        icon="mdi-discord"
        label="Discord"
        label-position="left"
        square
        target="_blank"
      />
      <q-fab-action
        :href="urls.gitbook"
        class="gitbook"
        color="secondary"
        icon="icon-gitbook"
        label="Documentation"
        label-position="left"
        square
        target="_blank"
      />
      <q-fab-action
        :href="urls.gitlab"
        color="secondary"
        icon="mdi-gitlab"
        label="Gitlab"
        label-position="left"
        square
        target="_blank"
      />
    </q-fab>
  </q-page-sticky>
</template>

<script>
  export default {
    name: "HelpButton",
    data() {
      return {
        urls: {
          forum: process.env.FORUM_URL,
          discord: process.env.DISCORD_URL,
          gitbook: process.env.GITBOOK_URL,
          gitlab: process.env.GITLAB_URL,
        },
      }
    },
  }
</script>

<style lang="sass">
  .q-page-container
    #help-button
      .q-btn--fab-mini:hover
        .q-icon:before
          color: $accent

      .gitbook
        i
          font-size: 18px
          margin-left: 3px
          margin-right: 3px
</style>
