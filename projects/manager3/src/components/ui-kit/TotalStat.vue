<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <transition
    appear
    enter-active-class="animated flipInX"
  >
    <div class="col q-pa-md statContainer total">
      <div class="badge">
        <div
          :class="'circle-container' + (route ? ' hover' : '')"
          @click="click"
        >
          <div class="circle">
            <div class="value">{{ value }}</div>
            <div class="title">
              {{ $t(title) }}
            </div>
          </div>
        </div>
      </div>
    </div>
  </transition>
</template>

<script lang="ts" setup>
import { toRefs } from 'vue';
import { useRouter } from 'vue-router';

const router = useRouter();

const props = defineProps({
  title: { type: String },
  value: { type: Number, default: 0 },
  route: { type: String },
});

const {
  title, value, route,
} = toRefs(props);

function click() {
  if (route?.value) {
    router.push(
      { name: route.value },
    );
  }
}
</script>

<style lang="sass" scoped>
.statContainer
  display: flex
  justify-content: center
  align-items: center

  .badge
    .circle-container
      background-color: $light-blue
      height: 120px
      width: 120px
      border-radius: 50%

      .circle
        height: 100%
        width: 100%
        background-image: url(../../assets/controller-background.png)
        display: flex
        flex-direction: column
        justify-content: center
        align-items: center
        border-radius: 50%

        .value,
        .title
          cursor: default
          color: white
          text-shadow: -1px 0 $secondary, 0 1px $secondary, 1px 0 $secondary, 0 -1px $secondary

        .title
          text-transform: uppercase

        .value
          font-size: 3em

    .hover
      &:hover
        cursor: pointer
        background-color: $accent

        .value,
        .title
          cursor: pointer
</style>
