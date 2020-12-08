<template>
  <div class="col-12 container"
       v-bind:style="{ maxHeight: height }"
  >
    <div
      :key="index"
      @click="copyToClipboard(index + ' : ' + value)"
      class="row version-line"
      v-for="(value, index) in data"
    >
      <div class="col col-md-6 text-uppercase self-center" style="text-align: right; padding-right: 5%;">
        <div>{{index}}</div>
      </div>
      <div class="col col-md-6" style="padding-left: 5%;">
        <q-chip color="light-blue" dense icon="mdi-source-branch" square text-color="white">
          {{value}}
        </q-chip>
      </div>
    </div>
  </div>
</template>

<script>
  import tools from 'src/tools'

  export default {
    name: 'LongDataList',
    methods: {
      copyToClipboard: (content) => tools.copyToClipboard(content),
    },
    props: {
      height: {
        type: String,
      },
      data: {
        type: Object,
        required: true,
      },
    },
  }
</script>

<style lang="sass">
  .container
    overflow-y: scroll
    overflow-x: hidden
    padding: 1em

    .version-line
      &:hover
        background: $rc-input-grey
        cursor: pointer

        .q-chip
          background: $primary !important
          color: $accent !important

    @media(max-width: 700px)
      .container
        .version-line
          .col-md-6
            padding-right: 0 !important
            padding-left: 0 !important
</style>
