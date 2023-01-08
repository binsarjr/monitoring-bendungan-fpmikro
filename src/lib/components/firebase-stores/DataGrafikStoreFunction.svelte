<script lang="ts">
	import { onValue, ref } from 'firebase/database';
	import moment from 'moment';
	import { onDestroy } from 'svelte';
	import { db } from '../../stores/database';
	import { dataCharts } from '../../stores/preferences';

	const off = onValue(ref($db, 'datalog'), (snapshot) => {
		const logs = snapshot.val() || {};
		let results: { x: string; y: number }[] = [];
		Object.keys(logs).map((key) => {
			const tinggiAir = logs[key];
			const date = moment.utc(key);
			results[results.length] = {
				x: date.utc().format('YYYY-MM-DD HH:mm:ss'),
				y: tinggiAir
			};
		});
		$dataCharts = results;
	});
	onDestroy(() => {
		off();
	});
</script>
