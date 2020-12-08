<template>
  <div class="row items-stretch line">
    <div class="col-12 col-md-4 title vertical-middle text-right">
      {{ $t(title) }}
    </div>
    <div class="col-12 col-md-8 content">
      <slot name="content"></slot>
    </div>
  </div>
</template>

<script>
  export default {
    name: 'FormFragmentContainer',
    props: {
      title: {
        type: String,
        required: true,
        default: 'title',
      },
    },
  }
</script>

<style lang="sass">
  .line
    margin: 0 8px 1em
    background: white

    .title
      text-transform: uppercase
      font-weight: 100
      font-size: x-large
      padding: .3em 1em

    .content
      padding: 1em
</style>
