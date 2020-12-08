<template>
  <div class="fixed-center text-center">
    <p>
      <img
        src="~assets/sad.svg"
        style="width:30vw;max-width:150px;"
      >
    </p>
    <p class="text-faded">
      {{ $t('404.message') }}<strong>(404)</strong>
    </p>
    <q-btn
      :label="$t('404.returnButton')"
      color="primary"
      style="width:200px;"
      to="/"
    />
  </div>
</template>

<script>
  export default {
    name: 'Error404',
  }
</script>
