/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { TWITCH } from 'src/router/api.routes';
import { runSequentialPromises } from 'quasar';
import axios from 'axios';

export type TwitchStoreState = {
  schedule: object,
};

export const useTwitchStore = defineStore('twitch', {
  state: () => ({
    schedule: {},
  } as TwitchStoreState),

  actions: {
    getSchedule():void {
      // eslint-disable-next-line @typescript-eslint/ban-ts-comment
      // @ts-ignore
      runSequentialPromises({
        auth: () => axios.post(TWITCH.auth, {
          client_id: 'jgp2r9ixktfq9fugog7wjeutk0dt21',
          client_secret: 'dnjccue3una7cccq6hxqpcjtxif69c',
          grant_type: 'client_credentials',
        }),
        schedule: (resultAggregator) => axios.create({
          headers: {
            // eslint-disable-next-line @typescript-eslint/ban-ts-comment
            // @ts-ignore
            Authorization: `Bearer ${resultAggregator.auth.value.data.access_token}`,
            'Client-Id': 'jgp2r9ixktfq9fugog7wjeutk0dt21',
          },
        }).get(TWITCH.schedule, { params: { broadcaster_id: '115060112' } }),
      }).catch((errResult):void => {
        // eslint-disable-next-line no-console
        console.error(`Error encountered on job (${errResult.key}):`);
        // eslint-disable-next-line no-console
        console.error(errResult.reason);
        // eslint-disable-next-line no-console
        console.log('Managed to get these results before this error:');
        // eslint-disable-next-line no-console
        console.log(errResult.resultAggregator);
      });
    },
  },
});
