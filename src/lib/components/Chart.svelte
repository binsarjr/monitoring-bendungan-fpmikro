<script lang="ts">
	import { Line } from 'svelte-chartjs';
	import {
		Chart as ChartJS,
		Title,
		Tooltip,
		Legend,
		LineElement,
		LinearScale,
		PointElement,
		CategoryScale,
		TimeSeriesScale,
		TimeScale,
		type ChartOptions
	} from 'chart.js';
	import 'chartjs-adapter-moment';
	import Annotation from 'chartjs-plugin-annotation';
	import { dataCharts } from '$lib/stores/preferences';
	import { minBahaya, minBencana, minSiaga } from '$lib/stores/iot';
	export let unit = 'second';
	export let max: string | undefined = undefined;
	export let min: string | number | undefined = undefined;

	const data = {
		datasets: [
			{
				label: 'Periode Ketinggian Air',

				fill: true,
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: $dataCharts
			}
		]
	};

	let options: any = {
		responsive: true,
		scales: {
			x: {
				type: 'timeseries',
				time: {
					unit,
					displayFormats: {
						second: 'mm:ss',
						minute: 'hh:mm'
					}
				}
			},
			y: {
				// min: 450,
				// max: 650
			}
		},
		plugins: {
			annotation: {
				common: {
					drawTime: 'beforeDatasetsDraw'
				},
				annotations: {
					bencana: {
						type: 'line',
						borderColor: 'black',
						borderWidth: 3,
						scaleID: 'y',
						value: $minBencana,
						// value: 560,
						label: {
							display: true,
							backgroundColor: 'black',
							borderColor: 'black',
							borderRadius: 10,
							borderWidth: 2,
							content: (ctx) => 'Bencana',
							rotation: 'auto',
							position: 'end'
						}
					},
					bahaya: {
						type: 'line',
						borderColor: 'red',
						borderWidth: 3,
						scaleID: 'y',
						value: $minBahaya,
						// value: 550,
						label: {
							display: true,
							backgroundColor: 'red',
							borderColor: 'red',
							borderRadius: 10,
							borderWidth: 2,
							content: (ctx) => 'Bahaya',
							rotation: 'auto'
						}
					},
					Siaga: {
						type: 'line',
						borderColor: 'yellow',
						borderWidth: 3,
						scaleID: 'y',
						value: $minSiaga,
						// value: 521,
						label: {
							display: true,
							backgroundColor: 'yellow',
							borderColor: 'yellow',
							color: 'black',
							borderRadius: 10,
							borderWidth: 2,
							content: (ctx) => 'Siaga',
							rotation: 'auto',
							position: 'start'
						}
					}
				}
			}
		}
	};
	$: if (min) {
		options.scales.x.min = min;
	}
	$: if (max) options.scales.x.max = max;

	$: if ($dataCharts) {
		data.datasets[0].data = $dataCharts;
	}

	ChartJS.register(
		Title,
		Tooltip,
		Legend,
		LineElement,
		LinearScale,
		PointElement,
		CategoryScale,
		TimeScale,
		TimeSeriesScale,
		Annotation
	);
</script>

<Line {data} {options} />
